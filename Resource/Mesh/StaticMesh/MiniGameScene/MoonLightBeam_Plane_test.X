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


Material MoonLightBeam_Wisp_Mat_INST_test {
 1.000000;1.000000;1.000000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "T_EV_LightBeam_Falloff_01.tga";
 }
}

Frame MoonLightBeam_Plane_test {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh MoonLightBeam_Plane_test {
  25;
  6.482155;-25.486563;0.000000;,
  11.330986;-19.090832;0.000000;,
  12.964306;-25.476352;0.000000;,
  0.000000;-25.496773;0.000000;,
  5.665493;-19.099758;0.000000;,
  9.697673;-12.705315;0.000000;,
  -11.330990;-19.126530;0.000000;,
  -6.482152;-25.506982;0.000000;,
  -12.964306;-25.517195;0.000001;,
  -5.665495;-19.117605;0.000000;,
  0.000000;-19.108680;0.000000;,
  4.848837;-12.712950;0.000000;,
  8.064358;-6.319794;-0.000001;,
  4.032180;-6.326146;-0.000001;,
  6.431046;0.065726;-0.000001;,
  -4.848837;-12.728229;0.000000;,
  0.000000;-12.720586;0.000000;,
  0.000000;-6.332498;0.000000;,
  -4.032180;-6.338850;0.000000;,
  3.215521;0.060663;-0.000001;,
  0.000000;0.055597;-0.000001;,
  -3.215522;0.050531;-0.000001;,
  -6.431043;0.045463;-0.000001;,
  -8.064360;-6.345200;0.000000;,
  -9.697674;-12.735866;0.000000;;
  32;
  3;0,1,2;,
  3;3,4,0;,
  3;1,0,4;,
  3;4,5,1;,
  3;6,7,8;,
  3;7,6,9;,
  3;9,3,7;,
  3;3,9,10;,
  3;4,3,10;,
  3;10,11,4;,
  3;5,4,11;,
  3;11,12,5;,
  3;12,11,13;,
  3;13,14,12;,
  3;15,10,9;,
  3;10,15,16;,
  3;11,10,16;,
  3;16,13,11;,
  3;13,16,17;,
  3;16,18,17;,
  3;18,16,15;,
  3;14,13,19;,
  3;17,19,13;,
  3;19,17,20;,
  3;17,21,20;,
  3;21,17,18;,
  3;18,22,21;,
  3;22,18,23;,
  3;15,23,18;,
  3;23,15,24;,
  3;9,24,15;,
  3;24,9,6;;

  MeshMaterialList {
   1;
   32;
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
   { MoonLightBeam_Wisp_Mat_INST_test }
  }

  DeclData {
   2;
   2;0;3;0;,
   1;0;5;0;;
   125;
   0,
   0,
   3212836864,
   1061158912,
   1065353216,
   0,
   0,
   3212836864,
   1065353216,
   1061158912,
   0,
   0,
   3212836864,
   1065353216,
   1065353216,
   0,
   0,
   3212836864,
   1056964608,
   1065353216,
   0,
   0,
   3212836864,
   1061158912,
   1061158912,
   0,
   0,
   3212836864,
   1065353216,
   1056964608,
   3022353057,
   3019601658,
   3212836864,
   939524096,
   1061158912,
   3022353057,
   3019601658,
   3212836864,
   1048576000,
   1065353216,
   3022353057,
   3019601658,
   3212836864,
   939524096,
   1065353216,
   0,
   0,
   3212836864,
   1048576000,
   1061158912,
   0,
   0,
   3212836864,
   1056964608,
   1061158912,
   0,
   0,
   3212836864,
   1061158912,
   1056964608,
   797409069,
   3022525880,
   3212836864,
   1065353216,
   1048576000,
   797414743,
   3022525886,
   3212836864,
   1061158912,
   1048576000,
   0,
   0,
   3212836864,
   1065353216,
   939524096,
   0,
   0,
   3212836864,
   1048576000,
   1056964608,
   0,
   0,
   3212836864,
   1056964608,
   1056964608,
   3028624773,
   0,
   3212836864,
   1056964608,
   1048576000,
   0,
   0,
   3212836864,
   1048576000,
   1048576000,
   0,
   0,
   3212836864,
   1061158912,
   939524096,
   797414862,
   3022525878,
   3212836864,
   1056964608,
   939524096,
   797414859,
   3022525878,
   3212836864,
   1048576000,
   939524096,
   797418375,
   3022525880,
   3212836864,
   939524096,
   939524096,
   797411323,
   3022525884,
   3212836864,
   939524096,
   1048576000,
   0,
   0,
   3212836864,
   939524096,
   1056964608;
  }
 }
}

AnimTicksPerSecond  {
 4800;
}

AnimationSet Anim1 {
 

 Animation {
  
  { MoonLightBeam_Plane_test }

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