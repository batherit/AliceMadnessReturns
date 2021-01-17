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


Material Facade2_BrickTileA_XXX_Mat_INST {
 1.000000;1.000000;1.000000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "Facade2_BrickTileA_DM.tga";
 }
}

Frame Facade2_WallLong_StyleA {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh Facade2_WallLong_StyleA {
  24;
  -0.319387;3.598911;4.802717;,
  -0.000608;-0.001094;5.120833;,
  -0.000608;3.598906;5.120833;,
  -0.319387;-0.001089;4.802717;,
  -0.319387;-0.001089;0.318950;,
  -0.319387;3.598911;4.802717;,
  -0.319387;3.598911;0.318950;,
  -0.319387;-0.001089;4.802717;,
  -0.319387;3.598911;0.318950;,
  -0.000608;3.598906;5.120833;,
  -0.000608;3.598906;0.000835;,
  -0.319387;3.598911;4.802717;,
  -0.000608;3.598906;0.000835;,
  -0.000608;-0.001094;5.120833;,
  -0.000608;-0.001094;0.000835;,
  -0.000608;3.598906;5.120833;,
  -0.319387;3.598911;0.318950;,
  -0.000608;-0.001094;0.000835;,
  -0.319387;-0.001089;0.318950;,
  -0.000608;3.598906;0.000835;,
  -0.319387;-0.001089;0.318950;,
  -0.000608;-0.001094;5.120833;,
  -0.319387;-0.001089;4.802717;,
  -0.000608;-0.001094;0.000835;;
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
   { Facade2_BrickTileA_XXX_Mat_INST }
  }

  DeclData {
   3;
   2;0;3;0;,
   1;0;5;0;,
   1;0;5;1;;
   168;
   3207910578,
   0,
   1060451623,
   1066827776,
   1048870912,
   1060954112,
   1032503296,
   3207910578,
   0,
   1060451624,
   1065697280,
   1065443328,
   1060052992,
   1056800768,
   3207910578,
   0,
   1060451623,
   1065697280,
   1048870912,
   1060052992,
   1032503296,
   3207910578,
   0,
   1060451623,
   1066827776,
   1065443328,
   1060954112,
   1056800768,
   3212836864,
   0,
   0,
   1074208768,
   1065443328,
   1059069952,
   1064886272,
   3212836864,
   0,
   0,
   1066827776,
   1048870912,
   1035952128,
   1057710080,
   3212836864,
   0,
   0,
   1074208768,
   1048870912,
   1059069952,
   1057710080,
   3212836864,
   0,
   0,
   1066827776,
   1065443328,
   1035952128,
   1064886272,
   931693915,
   1065353216,
   0,
   1025327104,
   1044692992,
   1038745600,
   1022738432,
   931693915,
   1065353216,
   0,
   1065697280,
   1048870912,
   1060052992,
   1032503296,
   931693915,
   1065353216,
   0,
   3169198080,
   1048870912,
   1033674752,
   1032503296,
   931693915,
   1065353216,
   0,
   1064935424,
   1044692992,
   1059422208,
   1022738432,
   1065353216,
   0,
   0,
   3169198080,
   1048870912,
   1033674752,
   1032503296,
   1065353216,
   0,
   0,
   1065697280,
   1065443328,
   1060052992,
   1056800768,
   1065353216,
   0,
   0,
   3169198080,
   1065443328,
   1033674752,
   1056800768,
   1065353216,
   0,
   0,
   1065697280,
   1048870912,
   1060052992,
   1032503296,
   3207910557,
   0,
   3207935292,
   3189571584,
   1048870912,
   1018978304,
   1032503296,
   3207910556,
   0,
   3207935293,
   3169198080,
   1065443328,
   1033674752,
   1056800768,
   3207910557,
   0,
   3207935293,
   3189571584,
   1065443328,
   1018978304,
   1056800768,
   3207910557,
   0,
   3207935293,
   3169198080,
   1048870912,
   1033674752,
   1032503296,
   3078853253,
   3212836864,
   0,
   1025327104,
   1066000384,
   1038745600,
   1057513472,
   3078853253,
   3212836864,
   0,
   1065697280,
   1065443328,
   1060052992,
   1056800768,
   3078853253,
   3212836864,
   0,
   1064935424,
   1066000384,
   1059422208,
   1057513472,
   3078853253,
   3212836864,
   0,
   3169198080,
   1065443328,
   1033674752,
   1056800768;
  }
 }
}

AnimTicksPerSecond  {
 4800;
}

AnimationSet Bigguy_LSP_Walk {
 

 Animation {
  
  { Facade2_WallLong_StyleA }

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
  
  { Facade2_WallLong_StyleA }

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
  
  { Facade2_WallLong_StyleA }

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
  
  { Facade2_WallLong_StyleA }

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
  
  { Facade2_WallLong_StyleA }

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
  
  { Facade2_WallLong_StyleA }

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