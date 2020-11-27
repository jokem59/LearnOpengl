# LearnOpengl

## Notes
  - Windows x64 compiled GLFW library and include files are located in this repo because I'm working on multiple machines
  - GLAD utility: https://glad.dav1d.de/

## OpenGL Notes
When passing VertexData to the GPU, this data gets stored in memory in the GPU and this area of memory is managed via the Vertex Buffer Object (VBO).  This is nice because the pipeline from CPU/Memory to GPU is relatively slow.  So by storing our VertexData in GPU memory, retrieval operations are very fast.  After binding a VBO with a certain buffer (e.g. GL\_BUFFER\_ARRAY), subsequent calls to GL\_BUFFE\R_ARRAY are associated with the bound VBO.

The Vertex Shader lets us speciy any input in the form of vertex attributes.  Though flexible, this means we must manually specify what part of our input data (VectorData) goes to which vertex attribute.  The more attributes you have, the more times you need to specify these attributes anytime you want to draw your object.  To "save" attribute state, we can use Vector Array Objects (VAO).

Some examples of how Vector Data can vary:
VBO1: Position Only
| pos[0] | pos[1] | pos[2] |

VBO2: Position and Velocity
| pos[0] | vel[0] | pos[1] | vel[1] |

VAOs can be bound just like VBOs, and any subsequenct vertex attribute calls from then on will be stored in the VAO.  The advantage with VAO is when configuring vertex attribute pointers, you only have to make the calls once and whenever we want to draw an object, just bind the corresponding VAO.  All the state is stored inside the VAO.

While VBO store a set of vertices, you can use an Element Buffer Object (EBO) to store a set of unique points, and then provide an array of indices to indicate the order you want to drawy our points in.  NOTE: OpenGL works in triangles so points and indices are specified in sets of three.  EBOs are bound to GL\_ELEMENT\_ARRAY\_BUFFER.  Luckily VAO keep track of EBO.  The last EBO buffer that's bound to a VAO is stored as the VAOs Element Buffer Object.  Binding a VAO automatically the corresponding EBO.  If you want your VAO to keep track of your EBO, unbind the VAO first followed by the EBO.  If you unbind the EBO first, it will unbind your EBO from the VAO.
