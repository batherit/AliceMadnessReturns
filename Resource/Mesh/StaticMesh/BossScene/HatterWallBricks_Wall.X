xof 0303txt 0032
template ColorRGBA {
 <35ff44e0-6c7c-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <d3e16e81-7835-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template Material {
 <3d82ab4d-62da-11cf-ab39-0020af71e433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template TextureFilename {
 <a42790e1-7810-11cf-8f52-0040333594a3>
 STRING filename;
}

template Frame {
 <3d82ab46-62da-11cf-ab39-0020af71e433>
 [...]
}

template Matrix4x4 {
 <f6f23f45-7686-11cf-8f52-0040333594a3>
 array FLOAT matrix[16];
}

template FrameTransformMatrix {
 <f6f23f41-7686-11cf-8f52-0040333594a3>
 Matrix4x4 frameMatrix;
}

template Vector {
 <3d82ab5e-62da-11cf-ab39-0020af71e433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template MeshFace {
 <3d82ab5f-62da-11cf-ab39-0020af71e433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
 <3d82ab44-62da-11cf-ab39-0020af71e433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template MeshMaterialList {
 <f6f23f42-7686-11cf-8f52-0040333594a3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material <3d82ab4d-62da-11cf-ab39-0020af71e433>]
}

template VertexElement {
 <f752461c-1e23-48f6-b9f8-8350850f336f>
 DWORD Type;
 DWORD Method;
 DWORD Usage;
 DWORD UsageIndex;
}

template DeclData {
 <bf22e553-292c-4781-9fea-62bd554bdd93>
 DWORD nElements;
 array VertexElement Elements[nElements];
 DWORD nDWords;
 array DWORD data[nDWords];
}

template AnimTicksPerSecond {
 <9e415a43-7ba6-4a73-8743-b73d47e88476>
 DWORD AnimTicksPerSecond;
}

template Animation {
 <3d82ab4f-62da-11cf-ab39-0020af71e433>
 [...]
}

template AnimationSet {
 <3d82ab50-62da-11cf-ab39-0020af71e433>
 [Animation <3d82ab4f-62da-11cf-ab39-0020af71e433>]
}

template FloatKeys {
 <10dd46a9-775b-11cf-8f52-0040333594a3>
 DWORD nValues;
 array FLOAT values[nValues];
}

template TimedFloatKeys {
 <f406b180-7b3b-11cf-8f52-0040333594a3>
 DWORD time;
 FloatKeys tfkeys;
}

template AnimationKey {
 <10dd46a8-775b-11cf-8f52-0040333594a3>
 DWORD keyType;
 DWORD nKeys;
 array TimedFloatKeys keys[nKeys];
}


Material HatterWallBricksB_XXX_Mat_INSt {
 1.000000;1.000000;1.000000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "HatterExtWallBrickB_DM.tga";
 }
}

Material material_1 {
 1.000000;1.000000;1.000000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "material_1";
 }
}

Material material_2 {
 1.000000;1.000000;1.000000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "material_2";
 }
}

Frame HatterWallBricks_Wall {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh HatterWallBricks_Wall {
  24;
  0.000000;10.240000;0.320000;,
  3.840000;0.000000;0.319999;,
  3.840000;10.240000;0.319999;,
  0.000000;0.000000;0.320000;,
  0.000000;10.240000;0.000000;,
  3.840000;10.240000;0.319999;,
  3.840000;10.240000;-0.000001;,
  0.000000;10.240000;0.320000;,
  0.000000;0.000000;0.000000;,
  3.840000;10.240000;-0.000001;,
  3.840000;0.000000;-0.000001;,
  0.000000;10.240000;0.000000;,
  0.000000;0.000000;0.320000;,
  3.840000;0.000000;-0.000001;,
  3.840000;0.000000;0.319999;,
  0.000000;0.000000;0.000000;,
  0.000000;10.240000;0.000000;,
  0.000000;0.000000;0.320000;,
  0.000000;10.240000;0.320000;,
  0.000000;0.000000;0.000000;,
  3.840000;10.240000;0.319999;,
  3.840000;0.000000;-0.000001;,
  3.840000;10.240000;-0.000001;,
  3.840000;0.000000;0.319999;;
  12;
  3;0,1,2;,
  3;1,0,3;,
  3;4,5,6;,
  3;5,4,7;,
  3;8,9,10;,
  3;9,8,11;,
  3;12,13,14;,
  3;13,12,15;,
  3;16,17,18;,
  3;17,16,19;,
  3;20,21,22;,
  3;21,20,23;;

  MeshMaterialList {
   3;
   12;
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0;
   { HatterWallBricksB_XXX_Mat_INSt }
   { material_1 }
   { material_2 }
  }

  DeclData {
   3;
   2;0;3;0;,
   1;0;5;0;,
   1;0;5;1;;
   168;
   881502891,
   0,
   1065353216,
   1076101120,
   3212681216,
   1064329216,
   1027391488,
   881502891,
   0,
   1065353216,
   1072398336,
   1064288256,
   1056866304,
   1064361984,
   881502891,
   0,
   1065353216,
   1072398336,
   3212681216,
   1056866304,
   1027391488,
   881502891,
   0,
   1065353216,
   1076101120,
   1064288256,
   1064329216,
   1064361984,
   0,
   1065353216,
   0,
   1065828352,
   3212681216,
   1016504320,
   1027391488,
   0,
   1065353216,
   0,
   1071890432,
   3213262848,
   1055629312,
   1016348672,
   0,
   1065353216,
   0,
   1071890432,
   3212681216,
   1055629312,
   1027391488,
   0,
   1065353216,
   0,
   1065828352,
   3213262848,
   1016504320,
   1016348672,
   3029061477,
   0,
   3212836864,
   1065828352,
   1064288256,
   1016504320,
   1064361984,
   3029061477,
   0,
   3212836864,
   1071890432,
   3212681216,
   1055629312,
   1027391488,
   3029061477,
   0,
   3212836864,
   1071890432,
   1064288256,
   1055629312,
   1064361984,
   3029061477,
   0,
   3212836864,
   1065828352,
   3212681216,
   1016504320,
   1027391488,
   0,
   3212836864,
   0,
   1065828352,
   1065295872,
   1016504320,
   1064828928,
   0,
   3212836864,
   0,
   1071890432,
   1064288256,
   1055629312,
   1064361984,
   0,
   3212836864,
   0,
   1071890432,
   1065295872,
   1055629312,
   1064828928,
   0,
   3212836864,
   0,
   1065828352,
   1064288256,
   1016504320,
   1064361984,
   3212836864,
   0,
   0,
   1076355072,
   3212681216,
   1064943616,
   1027391488,
   3212836864,
   0,
   0,
   1076101120,
   1064288256,
   1064329216,
   1064361984,
   3212836864,
   0,
   0,
   1076101120,
   3212681216,
   1064329216,
   1027391488,
   3212836864,
   0,
   0,
   1076355072,
   1064288256,
   1064943616,
   1064361984,
   1065353216,
   0,
   0,
   1072398336,
   3212681216,
   1056866304,
   1027391488,
   1065353216,
   0,
   0,
   1071890432,
   1064288256,
   1055629312,
   1064361984,
   1065353216,
   0,
   0,
   1071890432,
   3212681216,
   1055629312,
   1027391488,
   1065353216,
   0,
   0,
   1072398336,
   1064288256,
   1056866304,
   1064361984;
  }
 }
}

AnimTicksPerSecond  {
 4800;
}

AnimationSet Bolterfly_Walk_2 {
 

 Animation {
  
  { HatterWallBricks_Wall }

  AnimationKey S {
   1;
   1;
   0;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey R {
   0;
   1;
   0;4;1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey T {
   2;
   1;
   0;3;0.000000,0.000000,0.000000;;;
  }
 }
}

AnimationSet Bolterfly_Walk {
 

 Animation {
  
  { HatterWallBricks_Wall }

  AnimationKey S {
   1;
   1;
   0;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey R {
   0;
   1;
   0;4;1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey T {
   2;
   1;
   0;3;0.000000,0.000000,0.000000;;;
  }
 }
}

AnimationSet Bolterfly_Strafe_Rgt {
 

 Animation {
  
  { HatterWallBricks_Wall }

  AnimationKey S {
   1;
   1;
   0;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey R {
   0;
   1;
   0;4;1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey T {
   2;
   1;
   0;3;0.000000,0.000000,0.000000;;;
  }
 }
}

AnimationSet Bolterfly_Strafe_Lft {
 

 Animation {
  
  { HatterWallBricks_Wall }

  AnimationKey S {
   1;
   1;
   0;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey R {
   0;
   1;
   0;4;1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey T {
   2;
   1;
   0;3;0.000000,0.000000,0.000000;;;
  }
 }
}

AnimationSet Bolterfly_Sprint {
 

 Animation {
  
  { HatterWallBricks_Wall }

  AnimationKey S {
   1;
   1;
   0;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey R {
   0;
   1;
   0;4;1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey T {
   2;
   1;
   0;3;0.000000,0.000000,0.000000;;;
  }
 }
}

AnimationSet Bolterfly_Leeching_Detach {
 

 Animation {
  
  { HatterWallBricks_Wall }

  AnimationKey S {
   1;
   2;
   0;3;1.000000,1.000000,1.000000;;,
   5440;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey R {
   0;
   2;
   0;4;1.000000,0.000000,0.000000,0.000000;;,
   5440;4;1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey T {
   2;
   2;
   0;3;0.000000,0.000000,0.000000;;,
   5440;3;0.000000,0.000000,0.000000;;;
  }
 }
}

AnimationSet Bolterfly_Leeching_Attach {
 

 Animation {
  
  { HatterWallBricks_Wall }

  AnimationKey S {
   1;
   2;
   0;3;1.000000,1.000000,1.000000;;,
   960;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey R {
   0;
   2;
   0;4;1.000000,0.000000,0.000000,0.000000;;,
   960;4;1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey T {
   2;
   2;
   0;3;0.000000,0.000000,0.000000;;,
   960;3;0.000000,0.000000,0.000000;;;
  }
 }
}

AnimationSet Bolterfly_Leeching {
 

 Animation {
  
  { HatterWallBricks_Wall }

  AnimationKey S {
   1;
   2;
   0;3;1.000000,1.000000,1.000000;;,
   6400;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey R {
   0;
   2;
   0;4;1.000000,0.000000,0.000000,0.000000;;,
   6400;4;1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey T {
   2;
   2;
   0;3;0.000000,0.000000,0.000000;;,
   6400;3;0.000000,0.000000,0.000000;;;
  }
 }
}

AnimationSet Bolterfly_Idle_Damaged {
 

 Animation {
  
  { HatterWallBricks_Wall }

  AnimationKey S {
   1;
   2;
   0;3;1.000000,1.000000,1.000000;;,
   1600;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey R {
   0;
   2;
   0;4;1.000000,0.000000,0.000000,0.000000;;,
   1600;4;1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey T {
   2;
   2;
   0;3;0.000000,0.000000,0.000000;;,
   1600;3;0.000000,0.000000,0.000000;;;
  }
 }
}

AnimationSet Bolterfly_Attack_MeLee3 {
 

 Animation {
  
  { HatterWallBricks_Wall }

  AnimationKey S {
   1;
   2;
   0;3;1.000000,1.000000,1.000000;;,
   960;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey R {
   0;
   2;
   0;4;1.000000,0.000000,0.000000,0.000000;;,
   960;4;1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey T {
   2;
   2;
   0;3;0.000000,0.000000,0.000000;;,
   960;3;0.000000,0.000000,0.000000;;;
  }
 }
}