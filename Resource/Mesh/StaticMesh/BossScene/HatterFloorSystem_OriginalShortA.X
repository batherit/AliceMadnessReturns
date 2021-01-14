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


Material material_0 {
 1.000000;1.000000;1.000000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "material_0";
 }
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

Material HatterPlat_CheckerTile_RXX_Mat_INST {
 1.000000;1.000000;1.000000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "HatterPlat_CheckerTile_DM.tga";
 }
}

Frame HatterFloorSystem_OriginalShortA {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh HatterFloorSystem_OriginalShortA {
  24;
  -5.120082;2.560039;-5.120006;,
  -5.120081;0.000039;-0.000008;,
  -5.120081;2.560039;-0.000008;,
  -5.120082;0.000039;-5.120006;,
  -0.000082;0.000039;-5.120009;,
  -0.000081;2.560039;-0.000009;,
  -0.000081;0.000039;-0.000009;,
  -0.000082;2.560039;-5.120009;,
  -0.000082;2.560039;-5.120009;,
  -5.120082;0.000039;-5.120006;,
  -5.120082;2.560039;-5.120006;,
  -0.000082;0.000039;-5.120009;,
  -5.120081;2.560039;-0.000008;,
  -0.000081;0.000039;-0.000009;,
  -0.000081;2.560039;-0.000009;,
  -5.120081;0.000039;-0.000008;,
  -0.000082;2.560039;-5.120009;,
  -5.120081;2.560039;-0.000008;,
  -0.000081;2.560039;-0.000009;,
  -5.120082;2.560039;-5.120006;,
  -5.120082;0.000039;-5.120006;,
  -0.000081;0.000039;-0.000009;,
  -5.120081;0.000039;-0.000008;,
  -0.000082;0.000039;-5.120009;;
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
   1,
   1,
   1,
   1,
   1,
   1,
   1,
   1,
   2,
   2,
   2,
   2;
   { material_0 }
   { HatterWallBricksB_XXX_Mat_INSt }
   { HatterPlat_CheckerTile_RXX_Mat_INST }
  }

  DeclData {
   3;
   2;0;3;0;,
   1;0;5;0;,
   1;0;5;1;;
   168;
   3212836864,
   0,
   872415234,
   1065353216,
   1056784384,
   1055981568,
   1065132032,
   3212836864,
   0,
   872415234,
   939524096,
   1065353216,
   1065181184,
   1061167104,
   3212836864,
   0,
   872415234,
   939524096,
   1056784384,
   1065181184,
   1065132032,
   3212836864,
   0,
   872415234,
   1065353216,
   1065353216,
   1055981568,
   1061167104,
   1065353216,
   0,
   3025254143,
   939524096,
   1065353216,
   1065181184,
   1057021952,
   1065353216,
   0,
   3025254144,
   1065353216,
   1056784384,
   1055981568,
   1060986880,
   1065353216,
   0,
   3025254144,
   1065353216,
   1065353216,
   1055981568,
   1057021952,
   1065353216,
   0,
   3025254144,
   939524096,
   1056784384,
   1065181184,
   1060986880,
   3036676097,
   0,
   3212836864,
   1065353216,
   1056784384,
   1055981568,
   1048313856,
   3036676097,
   0,
   3212836864,
   939524096,
   1065353216,
   1065181184,
   1008975872,
   3036676097,
   0,
   3212836864,
   939524096,
   1056784384,
   1065181184,
   1048313856,
   3036676097,
   0,
   3212836864,
   1065353216,
   1065353216,
   1055981568,
   1008975872,
   877770520,
   0,
   1065353216,
   1065353216,
   1056784384,
   1055981568,
   1056735232,
   877770520,
   0,
   1065353216,
   939524096,
   1065353216,
   1065181184,
   1048805376,
   877770520,
   0,
   1065353216,
   939524096,
   1056784384,
   1065181184,
   1056735232,
   877770520,
   0,
   1065353216,
   1065353216,
   1065353216,
   1055981568,
   1048805376,
   0,
   1065353216,
   0,
   1065353216,
   939524096,
   1055604736,
   1057062912,
   0,
   1065353216,
   0,
   939524096,
   1065353216,
   1008975872,
   1065181184,
   0,
   1065353216,
   0,
   939524096,
   939524096,
   1008975872,
   1057062912,
   0,
   1065353216,
   0,
   1065353216,
   1065353216,
   1055604736,
   1065181184,
   0,
   3212836864,
   0,
   1065353216,
   1065353216,
   1055604736,
   1011122176,
   0,
   3212836864,
   0,
   939524096,
   939524096,
   1008975872,
   1056833536,
   0,
   3212836864,
   0,
   939524096,
   1065353216,
   1008975872,
   1011122176,
   0,
   3212836864,
   0,
   1065353216,
   939524096,
   1055604736,
   1056833536;
  }
 }
}

AnimTicksPerSecond  {
 4800;
}

AnimationSet Bolterfly_Walk_2 {
 

 Animation {
  
  { HatterFloorSystem_OriginalShortA }

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
  
  { HatterFloorSystem_OriginalShortA }

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
  
  { HatterFloorSystem_OriginalShortA }

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
  
  { HatterFloorSystem_OriginalShortA }

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
  
  { HatterFloorSystem_OriginalShortA }

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
  
  { HatterFloorSystem_OriginalShortA }

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
  
  { HatterFloorSystem_OriginalShortA }

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
  
  { HatterFloorSystem_OriginalShortA }

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
  
  { HatterFloorSystem_OriginalShortA }

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
  
  { HatterFloorSystem_OriginalShortA }

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