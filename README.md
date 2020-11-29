# LearnOpengl

## Notes
  - Windows x64 compiled GLFW library and include files are located in this repo because I'm working on multiple machines
  - GLAD utility: https://glad.dav1d.de/

## OpenGL Notes
### Vertex Buffer Objects - Managing GPU Memory
When passing VertexData to the GPU, this data gets stored in memory in the GPU and this area of memory is managed via the Vertex Buffer Object (VBO).  This is nice because the pipeline from CPU/Memory to GPU is relatively slow.  So by storing our VertexData in GPU memory, retrieval operations are very fast.  After binding a VBO with a certain buffer (e.g. GL\_BUFFER\_ARRAY), subsequent calls to GL\_BUFFE\R_ARRAY are associated with the bound VBO.

The Vertex Shader lets us speciy any input in the form of vertex attributes.  Though flexible, this means we must manually specify what part of our input data (VectorData) goes to which vertex attribute.  The more attributes you have, the more times you need to specify these attributes anytime you want to draw your object.  To "save" attribute state, we can use Vector Array Objects (VAO).

Some examples of how Vector Data can vary:
VBO1: Position Only
| pos[0] | pos[1] | pos[2] |

VBO2: Position and Velocity
| pos[0] | vel[0] | pos[1] | vec[1] |

### Vertex Array Objects - Managing VBO and their Attributes
VAOs can be bound just like VBOs, and any subsequent vertex attribute calls from then on will be stored in the VAO.  The advantage with VAO is when configuring vertex attribute pointers, you only have to make the calls once and whenever we want to draw an object, just bind the corresponding VAO.  All the state is stored inside the VAO.

### Element Buffer Objects - Like a VBO but pass in indices to draw point in specified order
While VBO store a set of vertices, you can use an Element Buffer Object (EBO) to store a set of unique points, and then provide an array of indices to indicate the order you want to drawy our points in.  NOTE: OpenGL works in triangles so points and indices are specified in sets of three.  EBOs are bound to GL\_ELEMENT\_ARRAY\_BUFFER.  Luckily VAO keep track of EBO.  The last EBO buffer that's bound to a VAO is stored as the VAOs Element Buffer Object.  Binding a VAO automatically the corresponding EBO.  If you want your VAO to keep track of your EBO, unbind the VAO first followed by the EBO.  If you unbind the EBO first, it will unbind your EBO from the VAO.

### GLSL
Typical shader language structure:

    #version version_number
    in type in_variable_name;
    in type in_variable_name;
    
    out type out_variable_name;
    
    uniform type uniform_name;
    
    void main()
    {
      // process input(s) and do some weird graphics stuff
      ...
      // output processed stuff to output variable
      out_variable_name = weird_stuff_we_processed;
    }

All GLSL start with a version declaration.

#### Types
GLSL has typical primitives in C: `int`, `float`, `double`, `uint`, and `bool`.  It also has two containers: `vector` and `matrices`.  Matrices will be discussed later.

##### Vector
GLSL can have 1, 2, 3, or 4 component vectors like:
  - `vecn`: default vector of `n` floats
  - `bvecn`: vector of `n` booleans
  - `ivecn`: vector of `n` ints
  - `uvecn`: vector of `n` unisgned integers
  - `dvecn`: vector of `n` double components

Elements of vectors can be accessed as such:
  - `vec.x` = first element
  - `vec.y` = second element
  - `vec.z` = third element
  - `vec.w` = fourth element

You can using _swizzling_ as syntatic sugar to quickly access choice elements in a vector.  NOTE: the right hand of the expression always folows the form of the `vecn` type, so `vec4` will always have `<vec>.x{4}`.

    vec2 twoVec;
    vec4 fourVec = someVec.xyxx; // (someVec.x, someVec.y, someVec.x, someVec.x)
    vec3 threeVec = fourVec.zyw; // (fourVec.z, fourVec.y, fourVec.w)
    vec4 otherFourVec = twoVec.xxxx + threeVec.yxzy;

You can also use _swizzling_ to fill in `vecn` constructors mixed with literals.

    vec2 vect = vec2(0.5, 0.7);
    vec4 result = vec4(vect, 0.0, 0.0);
    vec4 other = vec4(result.xyz, 1.0);


#### Ins and Outs
All shaders can specify *in* and *out* variables.  Specifically, an *in* variable is a *vertex attribute*, typically max of 16 but can be queried with `GL\_MAX\_VERTEX\_ATTRIBUTE`.  You _link_ variables from one shader to another by specifying the *out* variable in one shader and specifying the same variable name as *in* in the subsequent shader.

#### Uniforms
*Uniforms* are another means to pass data from the CPU to the shaders on the GPU.  These are different from *Vertex Attributes*.  *Uniforms* are _global_, which means a uniform variable is unique per shader object and can be accessed by any shader at any stage of the shader program.  They also differ because once you set their value, they keep that value until they are reset or updated.

*Uniform* variables can be accessed and set as follows using `glGetUniformLocation` and `glUniform4f`.  NOTE: the argument passed into `glGetUniformLocation` matches the *uniform* variable declaration in our shader program.

    float timeValue = glfwGetTime();
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    glUseProgram(shaderProgram);
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);


