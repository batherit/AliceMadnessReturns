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


Material Facade_RedBrickTile_Mat_INST {
 1.000000;1.000000;1.000000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "Facade_RedBrickTile_DM.tga";
 }
}

Material Facade_RedBrickTrim_Mat_INST {
 1.000000;1.000000;1.000000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "Facade_RedBrickTrim_DM.tga";
 }
}

Material material_3 {
 1.000000;1.000000;1.000000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "material_3";
 }
}

Frame Facade_WallSmall_Redbrick {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh Facade_WallSmall_Redbrick {
  24;
  0.000000;3.200000;0.000000;,
  0.320000;3.200000;-2.399631;,
  0.000000;3.200000;-2.560000;,
  0.320000;3.200000;-0.159639;,
  0.320000;0.000000;-0.159639;,
  0.000000;0.000000;-2.560000;,
  0.320000;0.000000;-2.399631;,
  0.000000;0.000000;0.000000;,
  0.000000;0.000000;-2.560000;,
  0.000000;3.200000;0.000000;,
  0.000000;3.200000;-2.560000;,
  0.000000;0.000000;0.000000;,
  0.320000;0.000000;-2.399631;,
  0.000000;3.200000;-2.560000;,
  0.320000;3.200000;-2.399631;,
  0.000000;0.000000;-2.560000;,
  0.000000;0.000000;0.000000;,
  0.320000;3.200000;-0.159639;,
  0.000000;3.200000;0.000000;,
  0.320000;0.000000;-0.159639;,
  0.320000;0.000000;-0.159639;,
  0.320000;3.200000;-2.399631;,
  0.320000;3.200000;-0.159639;,
  0.320000;0.000000;-2.399631;;
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
   4;
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
   { Facade_RedBrickTile_Mat_INST }
   { Facade_RedBrickTrim_Mat_INST }
   { Facade_RedBrickTrim_Mat_INST }
   { material_3 }
  }

  DeclData {
   3;
   2;0;3;0;,
   1;0;5;0;,
   1;0;5;1;;
   168;
   0,
   1065353216,
   0,
   1050222592,
   3199860736,
   1062338560,
   1059946496,
   0,
   1065353216,
   0,
   1053974528,
   1056071680,
   1061249024,
   1044807680,
   0,
   1065353216,
   0,
   1050222592,
   1057456128,
   1062338560,
   1042636800,
   0,
   1065353216,
   0,
   1053974528,
   3197992960,
   1061249024,
   1059405824,
   0,
   3212836864,
   0,
   1048649728,
   1056079872,
   1059848192,
   1047896064,
   0,
   3212836864,
   0,
   1041244160,
   3199860736,
   1060929536,
   1060716544,
   0,
   3212836864,
   0,
   1048649728,
   3197984768,
   1059848192,
   1060175872,
   0,
   3212836864,
   0,
   1041244160,
   1057456128,
   1060929536,
   1045733376,
   3212836864,
   0,
   0,
   1062158336,
   1065353216,
   1007149056,
   1065197568,
   3212836864,
   0,
   0,
   1006936064,
   939524096,
   1059536896,
   1056120832,
   3212836864,
   0,
   0,
   1062158336,
   939524096,
   1059536896,
   1065197568,
   3212836864,
   0,
   0,
   1006936064,
   1065353216,
   1007149056,
   1056120832,
   1055221054,
   0,
   3211058722,
   1065926656,
   1061863424,
   1062567936,
   1042276352,
   1055221054,
   0,
   3211058722,
   1064640512,
   3198533632,
   1063649280,
   1062019072,
   1055221053,
   0,
   3211058723,
   1065926656,
   3198533632,
   1062567936,
   1062019072,
   1055221053,
   0,
   3211058723,
   1064640512,
   1061863424,
   1063649280,
   1042276352,
   1055166285,
   0,
   1063588767,
   1066303488,
   3198533632,
   1063976960,
   1042276352,
   1055166285,
   0,
   1063588767,
   1067237376,
   1061863424,
   1065058304,
   1062019072,
   1055166285,
   0,
   1063588767,
   1066303488,
   1061863424,
   1063976960,
   1062019072,
   1055166285,
   0,
   1063588767,
   1067237376,
   3198533632,
   1065058304,
   1042276352,
   1065353216,
   0,
   0,
   1073070080,
   1057456128,
   1010696192,
   1055825920,
   1065353216,
   0,
   0,
   1066516480,
   3205898240,
   1059594240,
   1013702656,
   1065353216,
   0,
   0,
   1073070080,
   3205898240,
   1059594240,
   1055825920,
   1065353216,
   0,
   0,
   1066516480,
   1057456128,
   1010696192,
   1013702656;
  }
 }
}

AnimTicksPerSecond  {
 4800;
}

AnimationSet Bigguy_LSP_Walk {
 

 Animation {
  
  { Facade_WallSmall_Redbrick }

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

AnimationSet BigGuy_LSP_Talk_b {
 

 Animation {
  
  { Facade_WallSmall_Redbrick }

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

AnimationSet BigGuy_LSP_Talk_a {
 

 Animation {
  
  { Facade_WallSmall_Redbrick }

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

AnimationSet BigGuy_LSP_Sit_Idle {
 

 Animation {
  
  { Facade_WallSmall_Redbrick }

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

AnimationSet BigGuy_LSP_Idle_b {
 

 Animation {
  
  { Facade_WallSmall_Redbrick }

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

AnimationSet BigGuy_LSP_Idle_a {
 

 Animation {
  
  { Facade_WallSmall_Redbrick }

  AnimationKey S {
   1;
   2;
   0;3;1.000000,1.000000,1.000000;;,
   16000;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey R {
   0;
   2;
   0;4;1.000000,0.000000,0.000000,0.000000;;,
   16000;4;1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey T {
   2;
   2;
   0;3;0.000000,0.000000,0.000000;;,
   16000;3;0.000000,0.000000,0.000000;;;
  }
 }
}