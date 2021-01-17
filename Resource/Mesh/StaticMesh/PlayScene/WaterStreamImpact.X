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


Material WaterRipple_Mat_INST {
 1.000000;1.000000;1.000000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "WaterRipple_Mat_INST";
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

Frame WaterStreamImpact {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh WaterStreamImpact {
  20;
  -0.870154;0.000080;-0.282730;,
  0.914934;0.000080;0.000000;,
  0.870154;0.000080;-0.282729;,
  -0.740197;0.000080;-0.537785;,
  0.740198;0.000080;-0.537784;,
  -0.537784;0.000080;-0.740197;,
  0.537785;0.000080;-0.740196;,
  -0.282729;0.000080;-0.870154;,
  0.282731;0.000080;-0.870153;,
  0.000001;0.000080;-0.914934;,
  0.282730;0.000080;0.870154;,
  -0.282730;0.000080;0.870154;,
  0.000000;0.000080;0.914934;,
  -0.537785;0.000080;0.740197;,
  0.537785;0.000080;0.740197;,
  -0.740197;0.000080;0.537784;,
  0.740197;0.000080;0.537785;,
  -0.870154;0.000080;0.282730;,
  0.870154;0.000080;0.282730;,
  -0.914934;0.000080;0.000000;;
  18;
  3;0,1,2;,
  3;2,3,0;,
  3;3,2,4;,
  3;4,5,3;,
  3;5,4,6;,
  3;6,7,5;,
  3;7,6,8;,
  3;7,8,9;,
  3;10,11,12;,
  3;10,13,11;,
  3;13,10,14;,
  3;14,15,13;,
  3;15,14,16;,
  3;16,17,15;,
  3;17,16,18;,
  3;18,19,17;,
  3;19,18,1;,
  3;1,0,19;;

  MeshMaterialList {
   2;
   18;
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
   { WaterRipple_Mat_INST }
   { material_1 }
  }

  DeclData {
   2;
   2;0;3;0;,
   1;0;5;0;;
   100;
   0,
   1065353216,
   0,
   1059553280,
   1064935424,
   0,
   1065353216,
   0,
   1056964608,
   939524096,
   0,
   1065353216,
   0,
   1059553280,
   1019781120,
   0,
   1065353216,
   0,
   1061888000,
   1063747584,
   0,
   1065353216,
   0,
   1061888000,
   1036222464,
   0,
   1065353216,
   0,
   1063747584,
   1061888000,
   0,
   1065353216,
   0,
   1063747584,
   1045626880,
   0,
   1065353216,
   0,
   1064935424,
   1059553280,
   0,
   1065353216,
   0,
   1064935424,
   1051779072,
   0,
   1065353216,
   0,
   1065353216,
   1056964608,
   0,
   1065353216,
   0,
   1019781120,
   1051779072,
   0,
   1065353216,
   0,
   1019781120,
   1059553280,
   0,
   1065353216,
   0,
   939524096,
   1056964608,
   0,
   1065353216,
   0,
   1036222464,
   1061888000,
   0,
   1065353216,
   0,
   1036222464,
   1045626880,
   0,
   1065353216,
   0,
   1045626880,
   1063747584,
   0,
   1065353216,
   0,
   1045626880,
   1036222464,
   0,
   1065353216,
   0,
   1051779072,
   1064935424,
   0,
   1065353216,
   0,
   1051779072,
   1019781120,
   0,
   1065353216,
   0,
   1056964608,
   1065353216;
  }
 }
}

AnimTicksPerSecond  {
 4800;
}

AnimationSet Anim1 {
 

 Animation {
  
  { WaterStreamImpact }

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