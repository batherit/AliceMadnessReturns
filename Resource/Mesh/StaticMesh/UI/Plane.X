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


Material PlayerMenuBackground_Mat {
 1.000000;1.000000;1.000000;1.000000;;
 10.000000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "PlayerMenuBackground.tga";
 }
}

Frame Plane {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh Plane {
  16;
  -0.000000;0.016000;-0.016000;,
  -0.000000;0.016000;-0.005333;,
  -0.000000;0.005333;-0.016000;,
  0.000000;0.005333;0.016000;,
  0.000000;0.005333;0.005333;,
  0.000000;0.016000;0.016000;,
  0.000000;0.016000;0.005333;,
  -0.000000;0.005333;-0.005333;,
  -0.000000;-0.005333;-0.016000;,
  -0.000000;-0.005333;-0.005333;,
  -0.000000;-0.016000;-0.016000;,
  -0.000000;-0.016000;-0.005333;,
  0.000000;-0.005333;0.005333;,
  0.000000;-0.016000;0.005333;,
  0.000000;-0.005333;0.016000;,
  0.000000;-0.016000;0.016000;;
  18;
  3;0,1,2;,
  3;3,4,5;,
  3;6,5,4;,
  3;4,7,6;,
  3;1,6,7;,
  3;7,2,1;,
  3;2,7,8;,
  3;9,8,7;,
  3;8,9,10;,
  3;11,10,9;,
  3;9,12,11;,
  3;13,11,12;,
  3;12,14,13;,
  3;15,13,14;,
  3;7,4,9;,
  3;12,9,4;,
  3;4,3,12;,
  3;14,12,3;;

  MeshMaterialList {
   1;
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
   { PlayerMenuBackground_Mat }
  }

  DeclData {
   2;
   2;0;3;0;,
   1;0;5;0;;
   80;
   1065353216,
   671358688,
   3012357562,
   939524096,
   939524096,
   1065353216,
   671358688,
   3012357562,
   1051369472,
   939524096,
   1065353216,
   671358688,
   3012357562,
   939524096,
   1051369472,
   1065353216,
   679747295,
   3012357561,
   1065353216,
   1051369472,
   1065353216,
   679747295,
   3012357561,
   1059758080,
   1051369472,
   1065353216,
   679747295,
   3012357561,
   1065353216,
   939524096,
   1065353216,
   662970078,
   3012357560,
   1059758080,
   939524096,
   1065353216,
   660766224,
   3010566024,
   1051369472,
   1051369472,
   1065353216,
   0,
   3012357561,
   939524096,
   1059758080,
   1065353216,
   662970080,
   3012357562,
   1051369472,
   1059758080,
   1065353216,
   679747295,
   3012357561,
   939524096,
   1065353216,
   1065353216,
   671358686,
   3012357560,
   1051369472,
   1065353216,
   1065353216,
   669154832,
   3010566024,
   1059758080,
   1059758080,
   1065353216,
   660766224,
   3010566024,
   1059758080,
   1065353216,
   1065353216,
   662970079,
   3012357561,
   1065353216,
   1059758080,
   1065353216,
   671358687,
   3012357561,
   1065353216,
   1065353216;
  }
 }
}

AnimTicksPerSecond  {
 4800;
}

AnimationSet ClockworkBomb_Spawn_Put {
 

 Animation {
  
  { Plane }

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

AnimationSet ClockworkBomb_Spawn {
 

 Animation {
  
  { Plane }

  AnimationKey S {
   1;
   2;
   0;3;1.000000,1.000000,1.000000;;,
   160;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey R {
   0;
   2;
   0;4;1.000000,0.000000,0.000000,0.000000;;,
   160;4;1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey T {
   2;
   2;
   0;3;0.000000,0.000000,0.000000;;,
   160;3;0.000000,0.000000,0.000000;;;
  }
 }
}

AnimationSet ClockworkBomb_Land {
 

 Animation {
  
  { Plane }

  AnimationKey S {
   1;
   2;
   0;3;1.000000,1.000000,1.000000;;,
   8800;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey R {
   0;
   2;
   0;4;1.000000,0.000000,0.000000,0.000000;;,
   8800;4;1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey T {
   2;
   2;
   0;3;0.000000,0.000000,0.000000;;,
   8800;3;0.000000,0.000000,0.000000;;;
  }
 }
}

AnimationSet ClockworkBomb_Fall {
 

 Animation {
  
  { Plane }

  AnimationKey S {
   1;
   2;
   0;3;1.000000,1.000000,1.000000;;,
   1280;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey R {
   0;
   2;
   0;4;1.000000,0.000000,0.000000,0.000000;;,
   1280;4;1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey T {
   2;
   2;
   0;3;0.000000,0.000000,0.000000;;,
   1280;3;0.000000,0.000000,0.000000;;;
  }
 }
}

AnimationSet ClockworkBomb_ClockWork_Critica {
 

 Animation {
  
  { Plane }

  AnimationKey S {
   1;
   2;
   0;3;1.000000,1.000000,1.000000;;,
   1280;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey R {
   0;
   2;
   0;4;1.000000,0.000000,0.000000,0.000000;;,
   1280;4;1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey T {
   2;
   2;
   0;3;0.000000,0.000000,0.000000;;,
   1280;3;0.000000,0.000000,0.000000;;;
  }
 }
}

AnimationSet ClockworkBomb_ClockWork {
 

 Animation {
  
  { Plane }

  AnimationKey S {
   1;
   2;
   0;3;1.000000,1.000000,1.000000;;,
   3840;3;1.000000,1.000000,1.000000;;;
  }

  AnimationKey R {
   0;
   2;
   0;4;1.000000,0.000000,0.000000,0.000000;;,
   3840;4;1.000000,0.000000,0.000000,0.000000;;;
  }

  AnimationKey T {
   2;
   2;
   0;3;0.000000,0.000000,0.000000;;,
   3840;3;0.000000,0.000000,0.000000;;;
  }
 }
}