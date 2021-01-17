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


Material Facade_GreyBrickTile_MAT_INST {
 1.000000;1.000000;1.000000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "Facade_GreyBrickTile_DM.tga";
 }
}

Material Facade_GreyBrickTrim_MAT_INST {
 1.000000;1.000000;1.000000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "Facade_GreyBrickTrim_DM.tga";
 }
}

Material Facade_Glass_MAT_INST {
 1.000000;1.000000;1.000000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "Facade_Glass_DM.tga";
 }
}

Frame Facade_WallSmall_Greybrick {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh Facade_WallSmall_Greybrick {
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
   { Facade_GreyBrickTile_MAT_INST }
   { Facade_GreyBrickTrim_MAT_INST }
   { Facade_Glass_MAT_INST }
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
   1061765120,
   3205218304,
   1061167104,
   1062354944,
   0,
   1065353216,
   0,
   1063133184,
   1032273920,
   1060077568,
   1035632640,
   0,
   1065353216,
   0,
   1061765120,
   1037770752,
   1061167104,
   1026375680,
   0,
   1065353216,
   0,
   1063133184,
   3204538368,
   1060077568,
   1061535744,
   0,
   3212836864,
   0,
   3190308864,
   1032478720,
   1063903232,
   1034035200,
   0,
   3212836864,
   0,
   3195789312,
   3205201920,
   1064992768,
   1062158336,
   0,
   3212836864,
   0,
   3190308864,
   3204513792,
   1063903232,
   1061339136,
   0,
   3212836864,
   0,
   3195789312,
   1037942784,
   1064992768,
   1023057920,
   3212836864,
   0,
   0,
   1068662784,
   1065353216,
   1006723072,
   1065312256,
   3212836864,
   0,
   0,
   1058365440,
   961650688,
   1059643392,
   1056161792,
   3212836864,
   0,
   0,
   1068662784,
   961650688,
   1059643392,
   1065312256,
   3212836864,
   0,
   0,
   1058365440,
   1065353216,
   1006723072,
   1056161792,
   1055221054,
   0,
   3211058722,
   1057185792,
   1064468480,
   1061347328,
   1011449856,
   1055221054,
   0,
   3211058722,
   1054670848,
   1040564224,
   1062436864,
   1065115648,
   1055221053,
   0,
   3211058723,
   1057185792,
   1040564224,
   1061347328,
   1065115648,
   1055221053,
   0,
   3211058723,
   1054670848,
   1064468480,
   1062436864,
   1011449856,
   1055166285,
   0,
   1063588767,
   1060642816,
   1040482304,
   1062576128,
   1011449856,
   1055166285,
   0,
   1063588767,
   1062019072,
   1064443904,
   1063665664,
   1065115648,
   1055166285,
   0,
   1063588767,
   1060642816,
   1064443904,
   1062576128,
   1065115648,
   1055166285,
   0,
   1063588767,
   1062019072,
   1040482304,
   1063665664,
   1011449856,
   1065353216,
   0,
   0,
   1052000256,
   1064591360,
   1008656384,
   1055932416,
   1065353216,
   0,
   0,
   3194003456,
   1041055744,
   1059667968,
   1012195328,
   1065353216,
   0,
   0,
   1052000256,
   1041055744,
   1059667968,
   1055932416,
   1065353216,
   0,
   0,
   3194003456,
   1064591360,
   1008656384,
   1012195328;
  }
 }
}

AnimTicksPerSecond  {
 4800;
}

AnimationSet Bigguy_LSP_Walk {
 

 Animation {
  
  { Facade_WallSmall_Greybrick }

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
  
  { Facade_WallSmall_Greybrick }

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
  
  { Facade_WallSmall_Greybrick }

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
  
  { Facade_WallSmall_Greybrick }

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
  
  { Facade_WallSmall_Greybrick }

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
  
  { Facade_WallSmall_Greybrick }

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