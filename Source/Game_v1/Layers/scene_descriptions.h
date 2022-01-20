
#ifndef SCENE_DESCRIPTIONS_H
#define SCENE_DESCRIPTIONS_H

#include <string>

std::string scene_string =
R"""(
Scene: TestScene

Entities:
  - Entity: Camera
    CameraComponent:
      location: [0.0, 0.0, -1.0]
      orientation:
        f1: [1.0, 0.0, 0.0]
        f2: [0.0, 1.0, 0.0]
        f3: [0.0, 0.0, 1.0]
      scale: 1.0
      
  - Entity: Light
    LightComponent:
      location: [0.0, -1.0, 0.0]
      orientation:
        f1: [1.0, 0.0, 0.0]
        f2: [0.0, 1.0, 0.0]
        f3: [0.0, 0.0, 1.0]
      scale: 1.0

  - Entity: Skybox
    MeshComponent: Skybox

  - Entity: Plane
    TransformComponent:
      location: [1.0, 0.7, 1.0]
      orientation:
        f1: [1.0, 0.0, 0.0]
        f2: [0.0, 1.0, 0.0]
        f3: [0.0, 0.0, 1.0]
      scale: 0.5
    MeshComponent: Plane
    
  - Entity: Sphere
    TransformComponent:
      location: [-0.5, 1.5, 0.0]
      orientation:
        f1: [1.0, 0.0, 0.0]
        f2: [0.0, 1.0, 0.0]
        f3: [0.0, 0.0, 1.0]
      scale: 0.2
    MeshComponent: PurpleSphere

  - Entity: Sphere2
    TransformComponent:
      location: [0.5, 1.5, 0.0]
      orientation:
        f1: [1.0, 0.0, 0.0]
        f2: [0.0, 1.0, 0.0]
        f3: [0.0, 0.0, 1.0]
      scale: 0.25
    DynamicPropertiesComponent:
      inertial_mass: 1.0
      velocity: [0.001, 0.0, 0.0]
      angular_velocity: [0.0, 0.0, 0.0]
    MeshComponent: OrangeSphere
    
  - Entity: Earth
    TransformComponent:
      location: [5.5, 10.5, 0.0]
      orientation:
        f1: [1.0, 0.0, 0.0]
        f2: [0.0, 0.0, -1.0]
        f3: [0.0, 1.0, 0.0]
      scale: 2.3
    DynamicPropertiesComponent:
      inertial_mass: 5.0
      velocity: [0.0, 0.0, 0.0]
      angular_velocity: [0.0, 0.0, 0.0001]
    GravitationalMassComponent: 0.00001
    MeshComponent: Earth


Meshes:
  - Mesh: PurpleSphere
    MeshType: COLOURED_MESH
    vertex_file: "D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/meshes/01_colouredMeshes/Sphere_10_purple.txt"

  - Mesh: OrangeSphere
    MeshType: COLOURED_MESH
    vertex_file: "D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/meshes/01_colouredMeshes/Sphere_10_orange.txt"

  - Mesh: Plane
    MeshType: TEXTURED_MESH
    vertex_file: "D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/meshes/02_texturedMeshes/Plane_1.txt"
    texture_file: "D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/all_in_one.png"

  - Mesh: DeformedSphere
    MeshType: NORMAL_MESH
    vertex_file: "D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/meshes/03_normalMeshes/Sphere_8_deformed.txt"
    texture_file: "D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/all_in_one.png"

  - Mesh: Earth
    MeshType: NORMAL_MESH
    vertex_file: "D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/meshes/03_normalMeshes/Sphere_16_corrected.txt"
    texture_file: "D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/Earth_Realistic.png"

  - Mesh: Skybox
    MeshType: SKYBOX
    texture_file_f: "D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/space2_front.png"
    texture_file_b: "D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/space2_back.png"
    texture_file_l: "D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/space2_left.png"
    texture_file_r: "D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/space2_right.png"
    texture_file_u: "D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/space2_up.png"
    texture_file_d: "D:/cpp_codes/37_RealOrbit/Real_Orbit/assets/textures/skyboxes/space2_down.png"

)""";

#endif // SCENE_DESCRIPTIONS_H
