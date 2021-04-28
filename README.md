# Fun-With-PBR

## Features:
### Physical Based Rendering
Can use either textures or plain values to render objects using PBR. Also uses Image based lighting (IBL) so that an object's surroundings affects it's ambient lighting in a scene.
### Level building GUI
Uses ImGui to create a level building user interface. Structurally it is inspired by Unity's GUI with objects on the left and Components on the right. Components and Entities can be added and their values can be changed. Selected entities are given a green outline to stand out.
### Model loading using assimp
Since assimp doesn't fully support PBR, it needs to be tricked slightly when loading in textures (metallic maps need to be read as "specular", roughness maps as "shininess" etc).
### Scene Management
All of the entities, lighting data and skybox of a scene are stored in a Scene object. You can switch between different scenes. You can also save and load scenes as Json files in the src/saves/ folder.
