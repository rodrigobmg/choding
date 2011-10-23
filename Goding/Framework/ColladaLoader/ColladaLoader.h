#pragma once

#include <dae.h>
#include <dom.h>
#include <dom/domCOLLADA.h>

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <d3d9.h>
#include <d3dx9.h>

#include <vector>

using namespace std;

class COLLADALoader
{
private:
	//Dae file
	DAE dae;

	//Root node
	daeElement* root;
	
	//<library_visual_scenes> node
	daeElement* library_visual_scenes;

	//<library_animations> node
	daeElement* library_animations;

public:
	//Constructor
	COLLADALoader()
	{
		root = NULL;
		library_visual_scenes = NULL;
		library_animations = NULL;
	}

	//Load all the meshes from a file
	MeshManager* Load(string filename)
	{
		//Output object
		MeshManager* Meshes = new MeshManager();

		#pragma region //Open the file and get the root node
		root = dae.open(filename);

		//Check if import succeded
		if(!root)
		{
			cout << "Document import failed. \n";
			return Meshes;
		}
		#pragma endregion
		
		#pragma region //Get the library_visual_scenes
		library_visual_scenes = root->getDescendant("library_visual_scenes");

		//Check if there is a <library_visual_scenes>
		if(!library_visual_scenes)
		{
			cout << "<library_visual_scenes> not found.\n";
			return Meshes;
		}
		#pragma endregion

		#pragma region //Get the library_animations
		library_animations = root->getDescendant("library_animations");

		//Check if there is a <library_animations>
		if(!library_animations) cout << "<library_animations> not found.\n";
		#pragma endregion

		//Process <library_visual_scenes>
		processVisualScenes(Meshes);

		//processSkinnedMeshes
		processSkinnedMeshes(Meshes);

		//Process library_animations if it exists
		if(library_animations) processAnimations(Meshes);

		//Process the <geometry> node for each Static Mesh
		processGeometries(Meshes);

		//Compile vertex components into one buffer for each Morphing Mesh
		for(unsigned int i = 0; i < Meshes->MorphingMeshes.size(); i++)
		{
			//Sort Animations
			Meshes->MorphingMeshes[i]->sortAnimation();

			//Compile Base
			Meshes->MorphingMeshes[i]->Base->combineComponents();

			//Compile Targets
			for(unsigned int z = 0; z < Meshes->MorphingMeshes[i]->Targets.size(); z++)
			{
				Meshes->MorphingMeshes[i]->Targets[z]->combineComponents();
			}
		}

		//Compile vertex components into one buffer for each Skinned Mesh and also combine Animation Keyframes list
		for(unsigned int i = 0; i < Meshes->SkinnedMeshes.size(); i++) 
		{
			Meshes->SkinnedMeshes[i]->combineComponents();
			Meshes->SkinnedMeshes[i]->combineJointAnimations();
		}

		//Compile vertex components into one buffer for each Static Mesh
		for(unsigned int i = 0; i < Meshes->StaticMeshes.size(); i++) Meshes->StaticMeshes[i]->combineComponents();

		//Close the .dae file
		dae.close(filename);

		//Set the pointers back to NULL, safety precaution for Debug build
		root = NULL; library_visual_scenes = NULL; library_animations = NULL;

		//Return MeshManager
		return Meshes;
	}

private:
	//Process a <library_visual_scenes> node
	void processVisualScenes(MeshManager* Meshes)
	{
		//Get the <visual_scene> node
		daeElement* visual_scene = library_visual_scenes->getDescendant("visual_scene");

		//Get all the <node>'s for the <visual_scene>
		daeTArray<daeElementRef> nodes = visual_scene->getChildren();

		//Keep track of which nodes to process after morphing meshes pass
		vector<bool> process = vector<bool>();

		//Set each boolean to true at first for each node
		for(unsigned int i = 0; i < nodes.getCount(); i++) process.push_back(true);

		//Do a first pass for Morphing Meshes
		for(unsigned int i = 0; i < nodes.getCount(); i++)
		{
			//Get the name and the type, if they exist
			string Name = nodes[i]->getAttribute("name").data();
			string Type = nodes[i]->getAttribute("type").data();

			//Skip JOINT node's, only meshes
			if(Type == "JOINT") continue;

			#pragma region //Get <instance_controller>, <controller> and <morph>

			//Get the <instance_controller> node that corresponds to this <node> if it exists
			domInstance_controller* instance_controller = NULL;
			instance_controller = (domInstance_controller*)nodes[i]->getDescendant("instance_controller");

			//Only interested in finding the morphing meshes
			if(!instance_controller) continue;

			//Get the <controller> element
			daeElement* controller = NULL;
			controller = instance_controller->getUrl().getElement();

			//If no <controller> exists that is referenced, skip this
			if(!controller) continue;

			//Does it have a descendant of type <morph>?
			daeElement* morph = NULL;
			morph = controller->getDescendant("morph");

			//if no <morph> then this is a skinned mesh
			if(!morph) continue;

			#pragma endregion

			#pragma region //Get the <source> child of <morph> that contains the Target names

			//Initialize to NULL, debug build safety
			daeElement* targetNameSource = NULL;

			//Get the children of <morph>
			daeTArray<daeElementRef> sources = morph->getChildren();

			//Find from these children, the <source> that contains "targets" in it's id attribute
			for(unsigned int z = 0; z < sources.getCount(); z++)
			{
				//Get ID
				string ID = sources[z]->getAttribute("id").data();

				//Does it contain "targets" in the ID?
				if(ID.find("targets") != string::npos)
				{
					targetNameSource = sources[z];

					break;
				}
			}

			//No targets? Whats the point!
			if(!targetNameSource) continue;

			#pragma endregion

			//Get the names of each geometry node that is a target mesh
			vector<string> targets = processStringSource(targetNameSource->getDescendant("IDREF_array"));

			#pragma region //Get its base geometry

			#pragma region //Get the <geometry> node
			//Get the geometry node reference for this skin
			string geometryReference = morph->getAttribute("source").data();

			//Erase the "#" at the beginning of the name
			geometryReference.erase(0, 1);

			//Get the <geometry> element by searching using the reference
			daeElement* baseGeometry = NULL;
			baseGeometry = dae.getDatabase()->idLookup(geometryReference, instance_controller->getDocument());

			//If no <geometry> exists that is referenced, skip this
			if(!baseGeometry) continue;
			#pragma endregion

			//Initialize a new MorphingMesh
			MorphingMesh* mesh = new MorphingMesh(Name);

			//Set this node to not be processed later
			process[i] = false;
			
			//Set the Base geometry
			mesh->Base = new Mesh(Name + "Base", processMatrix(nodes[i]->getDescendant("matrix")));
			mesh->Base->geometry = baseGeometry;

			#pragma endregion
			
			//Now find each <node> in the <visual_scenes> whos <instance_geometry> corresponds with a target
			for(unsigned int z = 0; z < nodes.getCount(); z++)
			{
				#pragma region //Get the geometry name and the <instance_geometry>

				//The name of the <geometry> node for this mesh
				string geometryName;

				//Get <instance_geometry>
				domInstance_geometry* instance_geometry = NULL;
				instance_geometry = (domInstance_geometry*)nodes[z]->getDescendant("instance_geometry");

				//If there is no <instance_geometry>, this isn't a static mesh and we will skip it.
				if(!instance_geometry) continue;

				//Get it's source url
				geometryName = instance_geometry->getAttribute("url").data();

				//Remove the "#"
				geometryName.erase(0, 1);

				#pragma endregion

				//Does this match any of the Targets?
				for(unsigned int x = 0; x < targets.size(); x++)
				{
					//Check
					if(targets[x] == geometryName)
					{
						#pragma region //Get its <geometry> node

						//Get the <geometry> node that is referenced by the <instance_geometry>
						daeElement* geometry = instance_geometry->getUrl().getElement();

						//If the referenced node was not found, skip this node
						if(!geometry) continue;

						#pragma endregion

						//Add to the Targets for this mesh
						mesh->Targets.push_back(new Mesh(nodes[z]->getAttribute("name"), processMatrix(nodes[z]->getDescendant("matrix"))));
						mesh->Targets.back()->geometry = geometry;

						//Set this node so it's not processed later
						process[z] = false;
					}
				}
			}

			Meshes->MorphingMeshes.push_back(mesh);
		}

		//For each <node>...
		for(unsigned int i = 0; i < nodes.getCount(); i++)
		{
			//Process this node?
			if(!process[i]) continue;

			//Get the name and the type, if they exist
			string Name = nodes[i]->getAttribute("name").data();
			string Type = nodes[i]->getAttribute("type").data();

			//Skip JOINT node's, only meshes
			if(Type == "JOINT") continue;
			
			//Get the <instance_controller> node that corresponds to this <node> if it exists
			domInstance_controller* instance_controller = NULL;
			instance_controller = (domInstance_controller*)nodes[i]->getDescendant("instance_controller");

			//If instance_controller exists, then this node is for a skinned mesh, else its a static mesh
			if(instance_controller)
			{
				//Get the <controller> element
				daeElement* controller = instance_controller->getUrl().getElement();

				//If no <controller> exists that is referenced, skip this
				if(!controller) continue;

				//Get the <skin> element, if any
				daeElement* skin = controller->getDescendant("skin");

				//Check if this controller has a <skin>
				if(!skin) continue;

				#pragma region //Get the <geometry> node
				//Get the geometry node reference for this skin
				string geometryReference = skin->getAttribute("source").data();

				//Erase the "#" at the beginning of the name
				geometryReference.erase(0, 1);

				//Get the <geometry> element by searching using the reference
				daeElement* geometry = NULL;
				geometry = dae.getDatabase()->idLookup(geometryReference, instance_controller->getDocument());

				//If no <geometry> exists that is referenced, skip this
				if(!geometry) continue;
				#pragma endregion

				#pragma region //Find the root skeleton <node> element
				daeElement* rootJoint = NULL;
				
				//Get all the Joints affecting this mesh
				daeTArray<daeElementRef> Joints = nodes[i]->getDescendant("instance_controller")->getChildren();

				//Search all the nodes in this visual_scene for a node that matches one of the Joint's affecting this mesh
				for(unsigned int z = 0; z < nodes.getCount(); z++)
				{
					//Get the Type of this node
					string Type = nodes[z]->getAttribute("type").data();
					
					//If this node isn't a JOINT, skip it
					if(!(Type == "JOINT")) continue;

					//Get it's ID
					string ID = nodes[z]->getAttribute("id").data();

					//Check each Joint affecting the mesh to see if this JOINT node matches any of them
					for(unsigned int x = 0; x < Joints.getCount(); x++)
					{
						//Get the joint being referenced
						string jointName = Joints[x]->getCharData();

						//Remove the "#" at the start of it's name
						jointName.erase(0, 1);

						//Compare
						if(jointName == ID)
						{
							//The name is the same, so set rootNode
							rootJoint = nodes[z];
							
							//No need to keep searching
							break;
						}
					}

					//Found it, so leave
					if(rootJoint) break;
				}
				
				//If no rootJoint was found after all that effort, skip this mesh
				if(!rootJoint) continue;
				#pragma endregion

				//Add a new SkinnedMesh and set it's Name as well as it's RootTransform
				Meshes->SkinnedMeshes.push_back(new SkinnedMesh(Name, processMatrix(nodes[i]->getDescendant("matrix"))));

				//Set the COLLADA references for this mesh
				Meshes->SkinnedMeshes.back()->controller = controller;
				Meshes->SkinnedMeshes.back()->geometry = geometry;
				Meshes->SkinnedMeshes.back()->rootJoint = rootJoint;

				//Add the Root Joint for this mesh
				Meshes->SkinnedMeshes.back()->Joints.push_back(Joint(rootJoint->getAttribute("name").data(), rootJoint->getAttribute("sid").data(), rootJoint));
				Meshes->SkinnedMeshes.back()->Joints.back().bind_matrix = processMatrix(rootJoint->getChild("matrix"));
			}
			else
			{
				//Get the <instance_geometry> node that corresponds to this <node>
				domInstance_geometry* instance_geometry = NULL;
				instance_geometry = (domInstance_geometry*)nodes[i]->getDescendant("instance_geometry");

				//If there is no <instance_geometry>, this isn't a static mesh and we will skip it.
				if(!instance_geometry) continue;

				//Get the <geometry> node that is referenced by the <instance_geometry>
				daeElement* geometry = instance_geometry->getUrl().getElement();

				//If the referenced node was not found, skip this node
				if(!geometry) continue;

				//Now create a new Mesh, set it's <geometry> node and get it's World transform.
				Meshes->StaticMeshes.push_back(new Mesh(Name, processMatrix(nodes[i]->getDescendant("matrix"))));
				Meshes->StaticMeshes.back()->geometry = geometry;
			}
		}
	}

	//Process a string source
	vector<string> processStringSource(daeElement* source)
	{
		//Initialize Output array
		vector<string> Output = vector<string>();

		//Get raw string representation from the node
		string raw = source->getCharData();

		//Get the number of strings
		unsigned int count = atoi(source->getAttribute("count").data());

		//Set up stringstream
		stringstream stm(raw);

		//Extract each string
		for(unsigned int i = 0; i < count; i++)
		{
			string temp;

			stm >> temp;

			Output.push_back(temp);
		}

		//Return extracted strings
		return Output;
	}

	//Process each Skinned Mesh's <controller> and also build a skeleton for them
	void processSkinnedMeshes(MeshManager* Meshes)
	{
		//Foreach Skinned Mesh
		for(unsigned int i = 0; i < Meshes->SkinnedMeshes.size(); i++)
		{
			//Build it's skeleton
			buildSkeleton(Meshes->SkinnedMeshes[i]);

			//Process it's <controller>
			processController(Meshes->SkinnedMeshes[i]);
		}
	}

	//Build a Skeleton from a SkinnedMesh's rootJoint <node>
	void buildSkeleton(SkinnedMesh* Mesh)
	{
		processJoint(Mesh, Mesh->rootJoint);

		//Find each Joint's parent
		for(unsigned int i = 1; i < Mesh->Joints.size(); i++)
		{
			//Get the parent node's name in the COLLADA file
			string parentName = Mesh->Joints[i].Node->getParent()->getAttribute("name").data();

			//Check the array of Joints to see if any match the parentName
			for(unsigned int z = 0; z < Mesh->Joints.size(); z++)
			{
				//Get this joint node's name in the COLLADA file
				string jointName = Mesh->Joints[z].Node->getAttribute("name").data();

				//Compare names
				if(parentName == jointName)
				{
					//Set the parentIndex to z
					Mesh->Joints[i].parentIndex = z;

					//Set the parentJoint reference to the address of the parentJoint
					Mesh->Joints[i].parentJoint = &Mesh->Joints[z];

					//Go to the next Joint in the list
					break;
				}
			}
		}
	}

	//Process a Joint <node> and it's children recursively
	void processJoint(SkinnedMesh* Mesh, daeElement* joint)
	{
		//Get the children of this joint
		daeTArray<daeElementRef> Children = joint->getChildren();

		//For each child
		for(unsigned int i = 0; i < Children.getCount(); i++)
		{
			//Get the Type of element this Child node is
			string elementName = Children[i]->getElementName();

			//If this element isn't of type node, skip it
			if(elementName.find("node") == string::npos) continue;

			//Get this Joint's Name and SID
			string Name = Children[i]->getAttribute("name").data();
			string SID = Children[i]->getAttribute("sid").data();

			//Add a Joint to this Mesh's Joint list
			Mesh->Joints.push_back(Joint(Name, SID, Children[i]));

			//Get the bind_matrix for this Joint
			Mesh->Joints.back().bind_matrix = processMatrix(Children[i]->getDescendant("matrix"));

			//Process this Joint's children
			processJoint(Mesh, Children[i]);
		}
	}

	//Process a <controller> node
	void processController(SkinnedMesh* Mesh)
	{
		//Get the <skin> node
		daeElement* skin = Mesh->controller->getDescendant("skin");

		//Only process skin nodes;
		if(!skin) return;

		//Get the BindShape matrix
		Mesh->BindShape = processMatrix(Mesh->controller->getDescendant("bind_shape_matrix"));

		#pragma region //Get the elements that hold the data we want
		//These are the three nodes of interest for this skin
		daeElement* inversePoses = NULL;
		daeElement* rawWeights = NULL;
		daeElement* vertex_weights = skin->getDescendant("vertex_weights");

		//Get the Children
		daeTArray<daeElementRef> Children = skin->getChildren();

		//Get the nodes of interest by searching through the Children
		for(unsigned int i = 0; i < Children.getCount(); i++)
		{
			//Get the ID for this child
			string ID = Children[i]->getAttribute("id").data();

			//Check what type of data this node references then set it if it's what we are looking for
			if(ID.find("bind_poses") != string::npos) inversePoses = Children[i];
			if(ID.find("weights") != string::npos) rawWeights = Children[i];
		}

		//If any of the four elements were not found, return
		if(!inversePoses || !rawWeights || !vertex_weights) return;
		#pragma endregion

		//Get the inverse bind pose for each Joint
		processBindPoseArray(Mesh->Joints, inversePoses);

		//Get the raw weights
		vector<float> weights = processWeightsArray(rawWeights);

		#pragma region //Process the weights and indices to form coherent component arrays
		//Get the array of influence counter's per vertex
		string rawVCount = vertex_weights->getDescendant("vcount")->getCharData();

		//Get the array of Joint and it's weights
		string rawV = vertex_weights->getDescendant("v")->getCharData();

		//Get the number of Vertice's, should be the same as the number of Position's in the geometry later on
		unsigned int count = atoi(vertex_weights->getAttribute("count").data());

		//Set up stringstreams for the data
		stringstream vtm(rawVCount);
		stringstream stm(rawV);

		//Read into component array's
		for(unsigned int i = 0; i < count; i++)
		{
			unsigned int influenceCount = 0;

			//Get the number of bone's influencing this vertex
			vtm >> influenceCount;

			//Push back a new Index and Weight
			Mesh->boneIndices.push_back(Index());
			Mesh->Weights.push_back(Weight());

			//Read weights based on influences
			for(unsigned int z = 0; z < influenceCount; z++)
			{
				unsigned int boneIndex = 0;
				unsigned int weightIndex = 0;

				//Get the Bone that's being referenced
				stm >> boneIndex;

				//Get the Weight by indexing the rawWeights array, this gets the index
				stm >> weightIndex;

				//Add to the current boneIndices
				Mesh->boneIndices.back().Indices[z] = boneIndex;

				//Add to the current Weights by indexing weight array
				Mesh->Weights.back().Weights[z] = weights[weightIndex];
			}
		}
		#pragma endregion
	}

	//Process an element that contains raw inverse bind pose data
	void processBindPoseArray(vector<Joint> &Joints, daeElement* source)
	{
		//Get the <float_array> node
		daeElement* float_array = source->getChild("float_array");

		//Get the number of inverse bind poses, should be same as number of joints, more info in .pdf
		unsigned int count = atoi(source->getDescendant("accessor")->getAttribute("count").data());

		//Get the raw string representation
		string rawInverseBindPose = float_array->getCharData();

		//Set up a stringstream with the raw data
		stringstream stm(rawInverseBindPose);

		//Read each float, in groups of 16, unfortunately you can't just call the processMatrix() function
		for(unsigned int i = 0; i < count; i++)
		{
			D3DXMATRIX out;

			stm >> out.m[0][0];
			stm >> out.m[0][1];
			stm >> out.m[0][2];
			stm >> out.m[0][3];

			stm >> out.m[1][0];
			stm >> out.m[1][1];
			stm >> out.m[1][2];
			stm >> out.m[1][3];
			
			stm >> out.m[2][0];
			stm >> out.m[2][1];
			stm >> out.m[2][2];
			stm >> out.m[2][3];
			
			stm >> out.m[3][0];
			stm >> out.m[3][1];
			stm >> out.m[3][2];
			stm >> out.m[3][3];

			Joints[i].inv_bind_matrix = out;
		}
	}

	//Process an element that contains raw weights data
	vector<float> processWeightsArray(daeElement* source)
	{
		//Initialize output array
		vector<float> out = vector<float>();

		//Get <float_array>
		daeElement* float_array = source->getChild("float_array");

		//Get the number of floats in the array
		unsigned int count = atoi(float_array->getAttribute("count").data());

		//Get the raw string representation
		string rawWeights = float_array->getCharData();

		//Set up a stringstream with the raw data
		stringstream stm(rawWeights);

		//Read each float
		for(unsigned int i = 0; i < count; i++)
		{
			float temp = 0;

			stm >> temp;

			out.push_back(temp);
		}

		return out;
	}

	//Process a <library_animations> node
	void processAnimations(MeshManager* Meshes)
	{
		//Get each <animation>
		daeTArray<daeElementRef> animations = library_animations->getChildren();

		//Foreach <animation> node
		for(unsigned int i = 0; i < animations.getCount(); i++)
		{
			//Get it's ID
			string ID = animations[i]->getAttribute("id").data();

			//Check if any of the MorhpingMeshes has any Target that is affected by this node
			for(unsigned int z = 0; z < Meshes->MorphingMeshes.size(); z++)
			{
				for(unsigned int x = 0; x < Meshes->MorphingMeshes[z]->Targets.size(); x++)
				{
					//Check if you can find this targets name in the ID
					if(ID.find(Meshes->MorphingMeshes[z]->Targets[x]->Name) != string::npos)
					{
						//Process this <animation> node and store it in this mesh's Animation array
						processAnimation(Meshes->MorphingMeshes[z], x, animations[i]);
						break;
					}
				}
			}

			//Check if any of the SkinnedMeshes has any Joint that is affected by this node
			for(unsigned int z = 0; z < Meshes->SkinnedMeshes.size(); z++)
			{
				for(unsigned int x = 0; x < Meshes->SkinnedMeshes[z]->Joints.size(); x++)
				{
					//Check if you can find this joints name in the ID and if this <animation> node is for a matrix
					if((ID.find(Meshes->SkinnedMeshes[z]->Joints[x].Name) != string::npos) && (ID.find("matrix") != string::npos))
					{
						//Process this <animation> node and store it in this Joint's Animation array
						processAnimation(&Meshes->SkinnedMeshes[z]->Joints[x], x, animations[i]);
						break;
					}
				}
			}
		}
	}

	//Process an <animation> node for a Morph Target
	void processAnimation(MorphingMesh* mesh, unsigned int targetIndex, daeElement* animation)
	{
		//Get sources
		daeTArray<daeElementRef> sources = animation->getChildren();

		//Initialize Animation component arrays
		vector<float> Times = vector<float>();
		vector<float> Weights = vector<float>();

		//Foreach <source>
		for(unsigned int i = 0; i < sources.getCount(); i++)
		{
			//Get the ID
			string ID = sources[i]->getAttribute("id").data();

			//Process depending on type
			if(ID.find("input") != string::npos) //Get the Times array
			{
				//Get the <float_array>
				daeElement* float_array = sources[i]->getDescendant("float_array");

				//Get the number of float's to read
				unsigned int count = atoi(sources[i]->getDescendant("accessor")->getAttribute("count").data());

				//Get the raw string representation
				string rawTimes = float_array->getCharData();

				//Set up a stringstream to read the data
				stringstream stm(rawTimes);

				//Read each float
				for(unsigned int z = 0; z < count; z++)
				{
					float temp;

					stm >> temp;

					Times.push_back(temp);
				}
			}
			else if(ID.find("output") != string::npos) //Get the Weights array
			{
				//Get the <float_array>
				daeElement* float_array = sources[i]->getDescendant("float_array");

				//Get the number of float's to read
				unsigned int count = atoi(sources[i]->getDescendant("accessor")->getAttribute("count").data());

				//Get the raw string representation
				string rawWeights = float_array->getCharData();

				//Set up a stringstream to read the data
				stringstream stm(rawWeights);

				//Read each float
				for(unsigned int z = 0; z < count; z++)
				{
					float temp;

					stm >> temp;

					Weights.push_back(temp);
				}
			}
		}

		//Compile the animation component arrays into AnimationKey's
		for(unsigned int i = 0; i < Times.size(); i++)
		{
			mesh->Animations.push_back(MorphAnimationKey(Times[i], targetIndex, Weights[i]));
		}
	}

	//Process an <animation> node for a Joint
	void processAnimation(Joint* joint, int jointIndex, daeElement* animation)
	{
		//Get sources
		daeTArray<daeElementRef> sources = animation->getChildren();

		//Initialize Animation component arrays
		vector<float> Times = vector<float>();
		vector<D3DXMATRIX> Transforms = vector<D3DXMATRIX>();

		//Foreach <source>
		for(unsigned int i = 0; i < sources.getCount(); i++)
		{
			//Get the ID
			string ID = sources[i]->getAttribute("id").data();

			//Process depending on type
			if(ID.find("input") != string::npos) //Get the Times array
			{
				//Get the <float_array>
				daeElement* float_array = sources[i]->getDescendant("float_array");

				//Get the number of floats to read
				unsigned int count = atoi(sources[i]->getDescendant("accessor")->getAttribute("count").data());

				//Get the raw string representation
				string rawTimes = float_array->getCharData();

				//Set up a stringstream to read the data
				stringstream stm(rawTimes);

				//Read each float
				for(unsigned int z = 0; z < count; z++)
				{
					float temp;

					stm >> temp;

					Times.push_back(temp);
				}
			}
			else if(ID.find("output") != string::npos) //Get the Transform's array
			{
				//Get the <float_array>
				daeElement* float_array = sources[i]->getDescendant("float_array");

				//Get the number of floats to read
				int count = atoi(sources[i]->getDescendant("accessor")->getAttribute("count").data());

				//Get the raw string representation
				string rawData = float_array->getCharData();
				
				//Set up a stringstream to read the data
				stringstream stm(rawData);

				//Read each matrix
				for(int z = 0; z < count; z++)
				{
					D3DXMATRIX temp;

					stm >> temp.m[0][0];
					stm >> temp.m[0][1];
					stm >> temp.m[0][2];
					stm >> temp.m[0][3];

					stm >> temp.m[1][0];
					stm >> temp.m[1][1];
					stm >> temp.m[1][2];
					stm >> temp.m[1][3];
					
					stm >> temp.m[2][0];
					stm >> temp.m[2][1];
					stm >> temp.m[2][2];
					stm >> temp.m[2][3];
					
					stm >> temp.m[3][0];
					stm >> temp.m[3][1];
					stm >> temp.m[3][2];
					stm >> temp.m[3][3];

					Transforms.push_back(temp);
				}
			}
		}

		//Compile the animation component arrays into AnimationKeys
		for(unsigned int i = 0; i < Times.size(); i++)
		{
			joint->Animations.push_back(JointAnimationKey(Times[i], Transforms[i], jointIndex));
		}
	}

	//Process a <geometry> node for each Static Mesh
	void processGeometries(MeshManager* Meshes)
	{
		//Foreach Static Mesh...
		for(unsigned int i = 0; i < Meshes->StaticMeshes.size(); i++)
		{
			//Get the <mesh> node
			daeElement* mesh = Meshes->StaticMeshes[i]->geometry->getDescendant("mesh");

			//Get the <source> nodes
			daeTArray<daeElementRef> sources = mesh->getChildren();

			//Get the <triangles> node (yes it will be in the sources array above if you wish to find it that way)
			daeElement* triangles = mesh->getDescendant("triangles");

			//Process each <source> child
			for(unsigned int z = 0; z < sources.getCount(); z++) processSource(Meshes->StaticMeshes[i], sources[z]);

			//Process the <triangles> child
			processTriangles(Meshes->StaticMeshes[i], triangles);
		}

		//Foreach Skinned Mesh...
		for(unsigned int i = 0; i < Meshes->SkinnedMeshes.size(); i++)
		{
			//Get the <mesh> node
			daeElement* mesh = Meshes->SkinnedMeshes[i]->geometry->getDescendant("mesh");

			//Get the <source> nodes
			daeTArray<daeElementRef> sources = mesh->getChildren();

			//Get the <triangles> node (yes it will be in the sources array above if you wish to find it that way)
			daeElement* triangles = mesh->getDescendant("triangles");

			//Process each <source> child
			for(unsigned int z = 0; z < sources.getCount(); z++) processSource(Meshes->SkinnedMeshes[i], sources[z]);

			//Process the <triangles> child
			processTriangles(Meshes->SkinnedMeshes[i], triangles);
		}

		//Foreach Morphing Mesh...
		for(unsigned int i = 0; i < Meshes->MorphingMeshes.size(); i++)
		{
			#pragma region //Base geometry

			//Get the <mesh> node
			daeElement* mesh = Meshes->MorphingMeshes[i]->Base->geometry->getDescendant("mesh");

			//Get the <source> nodes
			daeTArray<daeElementRef> sources = mesh->getChildren();

			//Get the <triangles> node (yes it will be in the sources array above if you wish to find it that way)
			daeElement* triangles = mesh->getDescendant("triangles");

			//Process each <source> child
			for(unsigned int z = 0; z < sources.getCount(); z++) processSource(Meshes->MorphingMeshes[i]->Base, sources[z]);

			//Process the <triangles> child
			processTriangles(Meshes->MorphingMeshes[i]->Base, triangles);

			#pragma endregion

			//Target geometries
			for(unsigned int z = 0; z < Meshes->MorphingMeshes[i]->Targets.size(); z++)
			{
				//Get the <mesh> node
				daeElement* mesh = Meshes->MorphingMeshes[i]->Targets[z]->geometry->getDescendant("mesh");

				//Get the <source> nodes
				daeTArray<daeElementRef> sources = mesh->getChildren();

				//Get the <triangles> node (yes it will be in the sources array above if you wish to find it that way)
				daeElement* triangles = mesh->getDescendant("triangles");

				//Process each <source> child
				for(unsigned int x = 0; x < sources.getCount(); x++) processSource(Meshes->MorphingMeshes[i]->Targets[z], sources[x]);

				//Process the <triangles> child
				processTriangles(Meshes->MorphingMeshes[i]->Targets[z], triangles);
			}
		}
	}

	//Process a <source> node for Static Mesh
	void processSource(Mesh* mesh, daeElement* source)
	{
		//Get Positions
		if(source->getAttribute("name").find("position") != string::npos)
		{
			//Get the <float_array> node
			daeElement* float_array = source->getChild("float_array");

			//Get the number of raw float's contained
			unsigned int count = atoi(float_array->getAttribute("count").data());

			//Get the raw string representation
			string positionArray = float_array->getCharData();

			//Set up a stringstream to read from the raw array
			stringstream stm(positionArray);

			//Read each float, in groups of three
			for(unsigned int i = 0; i < (count / 3); i++)
			{
				float x, y, z;

				stm >> x;
				stm >> y;
				stm >> z;

				//Push this back as another Position component
				mesh->Positions.push_back(D3DXVECTOR3(x, y, z));
			}
			
			return;
		}

		//Get Normals
		if(source->getAttribute("name").find("normal") != string::npos)
		{
			//Get the <float_array> node
			daeElement* float_array = source->getChild("float_array");

			//Get the number of raw float's contained
			unsigned int count = atoi(float_array->getAttribute("count").data());

			//Get the raw string representation
			string normalsArray = float_array->getCharData();

			//Set up a stringstream to read from the raw array
			stringstream stm(normalsArray);

			//Read each float, in groups of three
			for(unsigned int i = 0; i < (count / 3); i++)
			{
				float x, y, z;

				stm >> x;
				stm >> y;
				stm >> z;

				//Push this back as another Position component
				mesh->Normals.push_back(D3DXVECTOR3(x, y, z));
			}

			return;
		}

		//Get UVs at layer0
		if(source->getAttribute("name").find("map1") != string::npos)
		{
			//Get the <float_array> node
			daeElement* float_array = source->getChild("float_array");

			//Get the number of raw float's contained
			unsigned int count = atoi(float_array->getAttribute("count").data());

			//Get the raw string representation
			string uvArray = float_array->getCharData();

			//Set up a stringstream to read from the raw array
			stringstream stm(uvArray);

			//Read each float, in groups of three
			for(unsigned int i = 0; i < (count / 2); i++)
			{
				float x, y;

				stm >> x;
				stm >> y;

				//Push this back as another Position component
				mesh->UVs.push_back(D3DXVECTOR2(x, y));
			}

			return;
		}

		//Get Tangents at layer0, the reason there are different naming schemes, this covers the ones I've come into contact with
		if(source->getAttribute("id").find("map1-tangents") != string::npos || source->getAttribute("id").find("textangents") != string::npos)
		{
			//Get the <float_array> node
			daeElement* float_array = source->getChild("float_array");

			//Get the number of raw float's contained
			unsigned int count = atoi(float_array->getAttribute("count").data());

			//Get the raw string representation
			string tangentsArray = float_array->getCharData();

			//Set up a stringstream to read from the raw array
			stringstream stm(tangentsArray);

			//Read each float, in groups of three
			for(unsigned int i = 0; i < (count / 3); i++)
			{
				float x, y, z;

				stm >> x;
				stm >> y;
				stm >> z;

				//Push this back as another Position component
				mesh->Tangents.push_back(D3DXVECTOR3(x, y, z));
			}

			return;
		}

		//Get BiTangents at layer0, read above about the different names
		if(source->getAttribute("id").find("map1-binormals") != string::npos || source->getAttribute("id").find("texbinormals") != string::npos)
		{
			//Get the <float_array> node
			daeElement* float_array = source->getChild("float_array");

			//Get the number of raw float's contained
			unsigned int count = atoi(float_array->getAttribute("count").data());

			//Get the raw string representation
			string biTangentsArray = float_array->getCharData();

			//Set up a stringstream to read from the raw array
			stringstream stm(biTangentsArray);

			//Read each float, in groups of three
			for(unsigned int i = 0; i < (count / 3); i++)
			{
				float x, y, z;

				stm >> x;
				stm >> y;
				stm >> z;

				//Push this back as another Position component
				mesh->BiTangents.push_back(D3DXVECTOR3(x, y, z));
			}

			return;
		}
	}
	//Process a <source> node for Skinned Mesh
	void processSource(SkinnedMesh* mesh, daeElement* source)
	{
		//Get Positions
		if(source->getAttribute("name").find("position") != string::npos)
		{
			//Get the <float_array> node
			daeElement* float_array = source->getChild("float_array");

			//Get the number of raw float's contained
			unsigned int count = atoi(float_array->getAttribute("count").data());

			//Get the raw string representation
			string positionArray = float_array->getCharData();

			//Set up a stringstream to read from the raw array
			stringstream stm(positionArray);

			//Read each float, in groups of three
			for(unsigned int i = 0; i < (count / 3); i++)
			{
				float x, y, z;

				stm >> x;
				stm >> y;
				stm >> z;

				//Push this back as another Position component
				mesh->Positions.push_back(D3DXVECTOR3(x, y, z));
			}
			
			return;
		}

		//Get Normals
		if(source->getAttribute("name").find("normal") != string::npos)
		{
			//Get the <float_array> node
			daeElement* float_array = source->getChild("float_array");

			//Get the number of raw float's contained
			unsigned int count = atoi(float_array->getAttribute("count").data());

			//Get the raw string representation
			string normalsArray = float_array->getCharData();

			//Set up a stringstream to read from the raw array
			stringstream stm(normalsArray);

			//Read each float, in groups of three
			for(unsigned int i = 0; i < (count / 3); i++)
			{
				float x, y, z;

				stm >> x;
				stm >> y;
				stm >> z;

				//Push this back as another Position component
				mesh->Normals.push_back(D3DXVECTOR3(x, y, z));
			}

			return;
		}

		//Get UVs at layer0
		if(source->getAttribute("name").find("map1") != string::npos)
		{
			//Get the <float_array> node
			daeElement* float_array = source->getChild("float_array");

			//Get the number of raw float's contained
			unsigned int count = atoi(float_array->getAttribute("count").data());

			//Get the raw string representation
			string uvArray = float_array->getCharData();

			//Set up a stringstream to read from the raw array
			stringstream stm(uvArray);

			//Read each float, in groups of three
			for(unsigned int i = 0; i < (count / 2); i++)
			{
				float x, y;

				stm >> x;
				stm >> y;

				//Push this back as another Position component
				mesh->UVs.push_back(D3DXVECTOR2(x, y));
			}

			return;
		}

		//Get Tangents at layer0, the reason there are different naming schemes, this covers the ones I've come into contact with
		if(source->getAttribute("id").find("map1-tangents") != string::npos || source->getAttribute("id").find("textangents") != string::npos)
		{
			//Get the <float_array> node
			daeElement* float_array = source->getChild("float_array");

			//Get the number of raw float's contained
			unsigned int count = atoi(float_array->getAttribute("count").data());

			//Get the raw string representation
			string tangentsArray = float_array->getCharData();

			//Set up a stringstream to read from the raw array
			stringstream stm(tangentsArray);

			//Read each float, in groups of three
			for(unsigned int i = 0; i < (count / 3); i++)
			{
				float x, y, z;

				stm >> x;
				stm >> y;
				stm >> z;

				//Push this back as another Position component
				mesh->Tangents.push_back(D3DXVECTOR3(x, y, z));
			}

			return;
		}

		//Get BiTangents at layer0, read above about the different names
		if(source->getAttribute("id").find("map1-binormals") != string::npos || source->getAttribute("id").find("texbinormals") != string::npos)
		{
			//Get the <float_array> node
			daeElement* float_array = source->getChild("float_array");

			//Get the number of raw float's contained
			unsigned int count = atoi(float_array->getAttribute("count").data());

			//Get the raw string representation
			string biTangentsArray = float_array->getCharData();

			//Set up a stringstream to read from the raw array
			stringstream stm(biTangentsArray);

			//Read each float, in groups of three
			for(unsigned int i = 0; i < (count / 3); i++)
			{
				float x, y, z;

				stm >> x;
				stm >> y;
				stm >> z;

				//Push this back as another Position component
				mesh->BiTangents.push_back(D3DXVECTOR3(x, y, z));
			}

			return;
		}
	}

	//Process a <triangles> node for Static Mesh
	void processTriangles(Mesh* mesh, daeElement* triangles)
	{
		//Get the <p> node
		daeElement* p = triangles->getDescendant("p");

		//Get the number of faces, multiply by 3 to get number of indices
		unsigned int count = atoi(triangles->getAttribute("count").data()) * 3;

		//Get the raw string representation
		string pArray = p->getCharData();

		//Set up a stringstream to read from the raw array
		stringstream stm(pArray);

		//Read each unsigned int
		for(unsigned int i = 0; i < count; i++)
		{
			unsigned int p = 0;

			stm >> p;

			mesh->Indices.push_back(p);
		}
	}

	//Process a <triangles> node for Skinned Mesh
	void processTriangles(SkinnedMesh* mesh, daeElement* triangles)
	{
		//Get the <p> node
		daeElement* p = triangles->getDescendant("p");

		//Get the number of faces, multiply by 3 to get number of indices
		unsigned int count = atoi(triangles->getAttribute("count").data()) * 3;

		//Get the raw string representation
		string pArray = p->getCharData();

		//Set up a stringstream to read from the raw array
		stringstream stm(pArray);

		//Read each unsigned int
		for(unsigned int i = 0; i < count; i++)
		{
			unsigned int p = 0;

			stm >> p;

			mesh->Indices.push_back(p);
		}
	}

	//Read a <matrix> node, better to have this as a generalized function, will read into OpenGL style, conversion to DirectX later...
	D3DXMATRIX processMatrix(daeElement* matrix)
	{
		D3DXMATRIX out;
		string world = matrix->getCharData();
		stringstream stm(world);

		#pragma region Read the matrix
		stm >> out.m[0][0];
		stm >> out.m[0][1];
		stm >> out.m[0][2];
		stm >> out.m[0][3];

		stm >> out.m[1][0];
		stm >> out.m[1][1];
		stm >> out.m[1][2];
		stm >> out.m[1][3];
		
		stm >> out.m[2][0];
		stm >> out.m[2][1];
		stm >> out.m[2][2];
		stm >> out.m[2][3];
		
		stm >> out.m[3][0];
		stm >> out.m[3][1];
		stm >> out.m[3][2];
		stm >> out.m[3][3];
		#pragma endregion

		return out;
	}
};
