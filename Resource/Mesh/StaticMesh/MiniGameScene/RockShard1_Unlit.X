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


Material CaveRock_XXX_Mat {
 1.000000;1.000000;1.000000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "CaveRock_XXX_Mat";
 }
}

Frame RockShard1_Unlit {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh RockShard1_Unlit {
  24;
  0.000298;-0.024004;-0.114671;,
  -0.149718;-0.032122;-0.015945;,
  -0.127014;-0.028399;-0.072797;,
  0.149718;-0.035430;0.114671;,
  -0.011991;0.034874;-0.095581;,
  -0.122537;0.013043;-0.006075;,
  0.103321;0.035430;0.083471;,
  -0.099864;0.020894;-0.065798;,
  -0.099864;0.020894;-0.065798;,
  -0.149718;-0.032122;-0.015945;,
  -0.122537;0.013043;-0.006075;,
  -0.099864;0.020894;-0.065798;,
  -0.127014;-0.028399;-0.072797;,
  -0.011991;0.034874;-0.095581;,
  -0.099864;0.020894;-0.065798;,
  -0.011991;0.034874;-0.095581;,
  0.000298;-0.024004;-0.114671;,
  -0.011991;0.034874;-0.095581;,
  0.149718;-0.035430;0.114671;,
  -0.011991;0.034874;-0.095581;,
  0.103321;0.035430;0.083471;,
  -0.122537;0.013043;-0.006075;,
  0.103321;0.035430;0.083471;,
  -0.122537;0.013043;-0.006075;;
  12;
  3;0,1,2;,
  3;1,0,3;,
  3;4,5,6;,
  3;5,4,7;,
  3;8,9,10;,
  3;9,11,12;,
  3;13,12,14;,
  3;12,15,16;,
  3;17,18,16;,
  3;18,19,20;,
  3;21,18,22;,
  3;18,23,9;;

  MeshMaterialList {
   1;
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
   { CaveRock_XXX_Mat }
  }

  DeclData {
   2;
   2;0;3;0;,
   1;0;5;0;;
   120;
   1047801940,
   3207142437,
   3208002866,
   1058906112,
   1064075264,
   3211172778,
   3201672404,
   1039527221,
   1065254912,
   1063698432,
   3205481281,
   1016102049,
   3209938418,
   1063550976,
   1065041920,
   1060946930,
   3189650319,
   1059608531,
   1058398208,
   1051836416,
   3188000387,
   1065156004,
   1034227267,
   1059848192,
   1049092096,
   3188000387,
   1065156004,
   1034227267,
   1063477248,
   1038172160,
   3188000386,
   1065156004,
   1034227266,
   1064083456,
   1058881536,
   3188000354,
   1065156004,
   1034227349,
   1061519360,
   1039204352,
   3209630153,
   1057609282,
   3195139899,
   1061519360,
   1039204352,
   3211172778,
   3201672404,
   1039527221,
   1064157184,
   1017470976,
   3209630153,
   1057609282,
   3195139897,
   1063477248,
   1038172160,
   3209795636,
   1056722632,
   3197559226,
   1061519360,
   1039204352,
   3205481281,
   1016102049,
   3209938418,
   1061224448,
   1005314048,
   3199391910,
   1050843104,
   3210849652,
   1059848192,
   1049092096,
   3199391910,
   1050843104,
   3210849652,
   1061519360,
   1039204352,
   3198108435,
   1047485814,
   3211514744,
   1059848192,
   1049092096,
   1047801940,
   3207142437,
   3208002866,
   1058021376,
   1049280512,
   1061970490,
   1051258868,
   3204510578,
   1059848192,
   1049092096,
   1060946930,
   3189650319,
   1059608531,
   1064919040,
   1061183488,
   1062035915,
   1050166590,
   3204743516,
   1059848192,
   1049092096,
   1062035915,
   1050166590,
   3204743516,
   1064083456,
   1058881536,
   3200332566,
   1042196282,
   1063893952,
   1063477248,
   1038172160,
   3200332566,
   1042196282,
   1063893952,
   1064083456,
   1058881536,
   3201063679,
   1025765436,
   1063944259,
   1063477248,
   1038172160;
  }
 }
}

AnimTicksPerSecond  {
 4800;
}

AnimationSet Anim1 {
 

 Animation {
  
  { RockShard1_Unlit }

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