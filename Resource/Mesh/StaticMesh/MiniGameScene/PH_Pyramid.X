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

Frame PH_Pyramid {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh PH_Pyramid {
  16;
  0.320000;0.000000;0.320000;,
  0.000000;1.280000;0.000000;,
  -0.320000;0.000000;0.320000;,
  -0.320000;0.000000;-0.320000;,
  0.320000;0.000000;0.320000;,
  -0.320000;0.000000;0.320000;,
  0.320000;0.000000;-0.320000;,
  -0.320000;0.000000;0.320000;,
  0.000000;1.280000;0.000000;,
  -0.320000;0.000000;-0.320000;,
  0.320000;0.000000;-0.320000;,
  0.000000;1.280000;0.000000;,
  0.320000;0.000000;0.320000;,
  -0.320000;0.000000;-0.320000;,
  0.000000;1.280000;0.000000;,
  0.320000;0.000000;-0.320000;;
  6;
  3;0,1,2;,
  3;3,4,5;,
  3;4,3,6;,
  3;7,8,9;,
  3;10,11,12;,
  3;13,14,15;;

  MeshMaterialList {
   1;
   6;
   0,
   0,
   0,
   0,
   0,
   0;
   { material_0 }
  }

  DeclData {
   2;
   2;0;3;0;,
   1;0;5;0;;
   80;
   0,
   1048075074,
   1064852290,
   1058488320,
   1065304064,
   0,
   1065353216,
   0,
   1051648000,
   3136020480,
   0,
   1048075074,
   1064852290,
   1035730944,
   1065304064,
   0,
   3212836864,
   0,
   1056645120,
   1057415168,
   0,
   3212836864,
   0,
   1064648704,
   1065254912,
   0,
   3212836864,
   0,
   1056645120,
   1065254912,
   0,
   3212836864,
   0,
   1064648704,
   1057415168,
   3212335938,
   1048075074,
   0,
   1058488320,
   1065345024,
   3212335938,
   1048075074,
   0,
   1051648000,
   976715776,
   3212335938,
   1048075074,
   0,
   1035730944,
   1065345024,
   1064852290,
   1048075074,
   0,
   1058578432,
   1065304064,
   1064852290,
   1048075074,
   0,
   1051820032,
   3136020480,
   1064852290,
   1048075074,
   0,
   1036410880,
   1065304064,
   0,
   1048075074,
   3212335938,
   1058578432,
   1065345024,
   0,
   1065353216,
   0,
   1051820032,
   976715776,
   0,
   1048075074,
   3212335938,
   1036410880,
   1065345024;
  }
 }
}

AnimTicksPerSecond  {
 4800;
}

AnimationSet Anim1 {
 

 Animation {
  
  { PH_Pyramid }

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