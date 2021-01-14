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


Material OrnateGrilleA_AHX_Mat_INST {
 1.000000;1.000000;1.000000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "OrnateGrille_Metal_DM.tga";
 }
}

Material OrnateGrilleA_GXX_Mat_INST {
 1.000000;1.000000;1.000000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "OrnateGrille_Glass_DM.tga";
 }
}

Frame OrnateGrilleB {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh OrnateGrilleB {
  12;
  0.982284;-1.610442;0.005542;,
  -0.982284;1.610442;0.005543;,
  -0.982284;-1.610442;0.005543;,
  0.982284;1.610442;0.005542;,
  0.982284;-1.610442;-0.006325;,
  -0.982284;1.610442;-0.006324;,
  0.982284;1.610442;-0.006325;,
  -0.982284;-1.610441;-0.006324;,
  0.991743;-1.560954;0.000000;,
  -0.972824;1.659929;0.000000;,
  -0.972824;-1.560954;0.000000;,
  0.991743;1.659929;0.000000;;
  6;
  3;0,1,2;,
  3;1,0,3;,
  3;4,5,6;,
  3;5,4,7;,
  3;8,9,10;,
  3;9,8,11;;

  MeshMaterialList {
   2;
   6;
   0,
   0,
   0,
   0,
   1,
   1;
   { OrnateGrilleA_AHX_Mat_INST }
   { OrnateGrilleA_GXX_Mat_INST }
  }

  DeclData {
   3;
   2;0;3;0;,
   1;0;5;0;,
   1;0;5;1;;
   84;
   889760061,
   0,
   1065353216,
   1057267712,
   1064984576,
   1057144832,
   1061691392,
   889760061,
   0,
   1065353216,
   1065148416,
   1018707968,
   1064960000,
   1016668160,
   889760061,
   0,
   1065353216,
   1065148416,
   1064984576,
   1064960000,
   1061691392,
   889760061,
   0,
   1065353216,
   1057267712,
   1018707968,
   1057144832,
   1016668160,
   3037243709,
   0,
   3212836864,
   1057267712,
   1064984576,
   1056473088,
   1061683200,
   3037243709,
   0,
   3212836864,
   1065148416,
   1018707968,
   1017208832,
   1016348672,
   3037243709,
   0,
   3212836864,
   1057267712,
   1018707968,
   1056473088,
   1016348672,
   3037243709,
   0,
   3212836864,
   1065148416,
   1064984576,
   1017208832,
   1061683200,
   0,
   0,
   1065353216,
   1057267712,
   1064984576,
   1020428288,
   1062019072,
   0,
   0,
   1065353216,
   1065148416,
   1018707968,
   1064935424,
   1065050112,
   0,
   0,
   1065353216,
   1065148416,
   1064984576,
   1020428288,
   1065050112,
   0,
   0,
   1065353216,
   1057267712,
   1018707968,
   1064935424,
   1062019072;
  }
 }
}

AnimTicksPerSecond  {
 4800;
}

AnimationSet Bolterfly_Walk_2 {
 

 Animation {
  
  { OrnateGrilleB }

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
  
  { OrnateGrilleB }

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
  
  { OrnateGrilleB }

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
  
  { OrnateGrilleB }

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
  
  { OrnateGrilleB }

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
  
  { OrnateGrilleB }

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
  
  { OrnateGrilleB }

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
  
  { OrnateGrilleB }

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
  
  { OrnateGrilleB }

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
  
  { OrnateGrilleB }

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