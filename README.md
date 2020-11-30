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

VBO2: Position and Color

    | pos[0] | col[0] | pos[1] | col[1] |

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


#### More Vertex Attributes
Previously we've only been using position data in our vetex data, now we're adding color.

    float vertices[] = {
        // positions         // colors
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
    };

Because we're sending more data to the vertex shader, we need to adjust.

    #version 330 core
    layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
    layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
    
    out vec3 ourColor; // output a color to the fragment shader

    void main()
    {
        gl_Position = vec4(aPos, 1.0);
        ourColor = aColor; // set ourColor to the input color we got from the vertex data
    }

We need to link the output variable `ourColor` from the vertex shader to the input variable `ourColor` in the fragment shader.

    #version 330 core
    out vec4 FragColor;
    in vec3 ourColor;
    
    void main()
    {
        FragColor = vec4(ourColor, 1.0);
    }

We can imagine VBO memory looking like this now (where each element is 4 bytes):

    | Vertex 1  | Vertex 2  | Vertex 3  |
    |X|Y|Z|R|G|B|X|Y|Z|R|G|B|X|Y|Z|R|G|B|
    
    |-----------> Position Stride = 24 bytes
    | Position Offset = 0
    |-----> Color Offest = 12 bytes
          |-----------> Color Stride = 24 bytes

Knowing the above structure, we can updated our vertex format via `glVertexAttribPointer`:

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // Need to enable for each attribute
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1); // Need to enable for each attribute

Where `glVertexAttribPointer` arguments are:
  1. Location (as defined in shader vertex)
  2. \# of values
  3. Type of value: `GL_FLOAT`
  4. Do not normalize values: `GL_FALSE`
  5. Stride: 6 * sizeof(float) = 24 bytes
  6. Offset: 3 * sizeof(float) = 12 bytes

### Textures
Through color interpolation, we can get some interesting images.  To make anything realistic, we'd have to set pixels one at a time which isn't feasible.  To overcome this, we use *textures*.

A *texture* is typically a 2D image (1D and 3D images as well) used to add details to an object.  Think of a texture as an image wrapped around an object, like brick sticker around a triangle.  Because images can have a lot detail, it's easier to add a lot of detail to an object this way than via vertices.

In order to map a texture to the triangle we need to tell each vertex of the triangle which part of the texture it corresponds to. Each vertex should thus have a texture coordinate associated with them that specifies what part of the texture image to sample from. Fragment interpolation then does the rest for the other fragments.

*Texture coordinates* range from 0 to 1 in the x and y axis (remember that we use 2D texture images). Retrieving the texture color using texture coordinates is called *sampling*. Texture coordinates start at (0,0) for the _lower left corner_ of a texture image to (1,1) for the _upper right corner_ of a texture image. The following shows how we map texture coordinates to the triangle:
  - Bottom Left Vertex = (0, 0)
  - Bottop Right Vertex = (1, 0)
  - Top Vertex = (0.5, 1)

The resulting texture coordinates look like:

    float texCoords[] = {
    0.0f, 0.0f,  // lower-left corner
    1.0f, 0.0f,  // lower-right corner
    0.5f, 1.0f   // top-center corner
    };

*Texture Sampling* has a loose interpretation and can be done in many different ways. It is thus our job to tell OpenGL how it should sample its textures.

#### Texture Wrapping
Texture coordinates usually range from (0,0) to (1,1) but what happens if we specify coordinates outside this range? The default behavior of OpenGL is to repeat the texture images (we basically ignore the integer part of the floating point texture coordinate), but there are more options OpenGL offers:
  - `GL\_REPEAT`: The default behavior for textures. Repeats the texture image.
  - `GL\_MIRRORED\_REPEAT`: Same as GL\_REPEAT but mirrors the image with each repeat.
  - `GL\_CLAMP\_TO\_EDGE`: Clamps the coordinates between 0 and 1. The result is that higher coordinates become clamped to the edge, resulting in a stretched edge pattern.
  - `GL\_CLAMP\_TO\_BORDER`: Coordinates outside the range are now given a user-specified border color.

Each of the aforementioned options can be set per coordinate axis (s, t (and r if you're using 3D textures) equivalent to x,y,z) with the glTexParameter* function:

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

The first argument specifies the texture target; we're working with 2D textures so the texture target is `GL\_TEXTURE\_2D`. The second argument requires us to tell what option we want to set and for which texture axis; we want to configure it for both the S and T axis. The last argument requires us to pass in the texture wrapping mode we'd like and in this case OpenGL will set its texture wrapping option on the currently active texture with GL\_MIRRORED\_REPEAT.

If we choose the `GL\_CLAMP\_TO\_BORDER` option we should also specify a border color. This is done using the fv equivalent of the glTexParameter function with `GL\_TEXTURE\_BORDER\_COLOR` as its option where we pass in a float array of the border's color value:

    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

#### Texture Filtering
`GL\_NEAREST` (also known as nearest neighbor or point filtering) is the default texture filtering method of OpenGL. When set to GL_NEAREST, OpenGL selects the texel that center is closest to the texture coordinate.

`GL\_LINEAR` (also known as (bi)linear filtering) takes an interpolated value from the texture coordinate's neighboring texels, approximating a color between the texels. The smaller the distance from the texture coordinate to a texel's center, the more that texel's color contributes to the sampled color.

`GL\_NEAREST` results in blocked patterns where we can clearly see the pixels that form the texture while `GL\_LINEAR` produces a smoother pattern where the individual pixels are less visible. `GL\_LINEAR` produces a more realistic output, but some developers prefer a more 8-bit look and as a result pick the `GL\_NEAREST` option.

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#### Mipmaps
Imagine we had a large room with thousands of objects, each with an attached texture. There will be objects far away that have the same high resolution texture attached as the objects close to the viewer. Since the objects are far away and probably only produce a few fragments, OpenGL has difficulties retrieving the right color value for its fragment from the high resolution texture, since it has to pick a texture color for a fragment that spans a large part of the texture. This will produce visible artifacts on small objects, not to mention the waste of memory bandwidth using high resolution textures on small objects.

To solve this issue OpenGL uses a concept called *mipmaps* that is basically a collection of texture images where each subsequent texture is twice as small compared to the previous one. The idea behind mipmaps should be easy to understand: after a certain distance threshold from the viewer, OpenGL will use a different mipmap texture that best suits the distance to the object. Because the object is far away, the smaller resolution will not be noticeable to the user. OpenGL is then able to sample the correct texels, and there's less cache memory involved when sampling that part of the mipmaps.

Creating a collection of mipmapped textures for each texture image is cumbersome to do manually, but luckily OpenGL is able to do all the work for us with a single call to `glGenerateMipmaps` after we've created a texture.

When switching between mipmaps levels during rendering OpenGL may show some artifacts like sharp edges visible between the two mipmap layers. Just like normal texture filtering, it is also possible to filter between mipmap levels using NEAREST and LINEAR filtering for switching between mipmap levels. To specify the filtering method between mipmap levels we can replace the original filtering methods with one of the following four options:

  - `GL\_NEAREST\_MIPMAP\_NEAREST`: takes the nearest mipmap to match the pixel size and uses nearest neighbor interpolation for texture sampling.
  - `GL\_LINEAR\_MIPMAP\_NEAREST`: takes the nearest mipmap level and samples that level using linear interpolation.
  - `GL\_NEAREST\_MIPMAP\_LINEAR`: linearly interpolates between the two mipmaps that most closely match the size of a pixel and samples the interpolated level via nearest neighbor interpolation.
  - `GL\_LINEAR\_MIPMAP\_LINEAR`: linearly interpolates between the two closest mipmaps and samples the interpolated level via linear interpolation.

Just like texture filtering we can set the filtering method to one of the 4 aforementioned methods using `glTexParameteri`:

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#### Loading and Creating Textures
Use an image-loading library that supports several popular formats and does all the hard work for us. A library like `stb_image.h`.

By defining `STB\_IMAGE\_IMPLEMENTATION` the preprocessor modifies the header file such that it only contains the relevant definition source code, effectively turning the header file into a .cpp file, and that's about it. Now simply include stb_image.h somewhere in your program and compile.

To load an image using `stb\_image.h` we use its `stbi_load` function:

    int width, height, nrChannels;
    unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

The function first takes as input the location of an image file. It then expects you to give three ints as its second, third and fourth argument that stb_image.h will fill with the resulting image's width, height and number of color channels. We need the image's width and height for generating textures later on.

#### Generating a Texture
Like any of the previous objects in OpenGL, textures are referenced with an ID; let's create one:

    unsigned int texture;
    glGenTextures(1, &texture);

The `glGenTextures` function first takes as input how many textures we want to generate and stores them in a unsigned int array given as its second argument (in our case just a single unsigned int). Just like other objects we need to bind it so any subsequent texture commands will configure the currently bound texture:

    glBindTexture(GL_TEXTURE_2D, texture);

Now that the texture is bound, we can start generating a texture using the previously loaded image data. Textures are generated with `glTexImage2D`:

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

This is a large function with quite a few parameters so we'll walk through them step-by-step:
  - The first argument specifies the texture target; setting this to `GL\_TEXTURE\_2D` means this operation will generate a texture on the currently bound texture object at the same target (so any textures bound to targets `GL\_TEXTURE\_1D` or `GL\_TEXTURE\_3D` will not be affected).
  - The second argument specifies the mipmap level for which we want to create a texture for if you want to set each mipmap level manually, but we'll leave it at the base level which is 0.
  - The third argument tells OpenGL in what kind of format we want to store the texture. Our image has only RGB values so we'll store the texture with RGB values as well.
  - The 4th and 5th argument sets the width and height of the resulting texture. We stored those earlier when loading the image so we'll use the corresponding variables.
  - The next argument should always be 0 (some legacy stuff).
  - The 7th and 8th argument specify the format and datatype of the source image. We loaded the image with RGB values and stored them as chars (bytes) so we'll pass in the corresponding values.
  - The last argument is the actual image data.

Once `glTexImage2D` is called, the currently bound texture object now has the texture image attached to it. However, currently it only has the base-level of the texture image loaded and if we want to use mipmaps we have to specify all the different images manually (by continually incrementing the second argument) or, we could call `glGenerateMipmap` after generating the texture. This will automatically generate all the required mipmaps for the currently bound texture.

After we're done generating the texture and its corresponding mipmaps, it is good practice to free the image memory:

    // stb header library freeing image data in memory
    stbi_image_free(data);

#### Applying Textures
For the upcoming sections we will use the rectangle shape drawn with `glDrawElements` from the final part of the Hello Triangle chapter. We need to inform OpenGL how to sample the texture so we'll have to update the vertex data with the texture coordinates:

    float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };

Since we've added an extra vertex attribute we again have to notify OpenGL of the new vertex format:

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture  coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

Note that we have to adjust the stride parameter of the previous two vertex attributes to 8 * sizeof(float) as well.

Next we need to alter the vertex shader to accept the texture coordinates as a vertex attribute and then forward the coordinates to the fragment shader:

    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    layout (location = 2) in vec2 aTexCoord;
    
    out vec3 ourColor;
    out vec2 TexCoord;
    
    void main()
    {
        gl_Position = vec4(aPos, 1.0);
        ourColor = aColor;
        TexCoord = aTexCoord;
    }

The fragment shader should then accept the `TexCoord` output variable as an input variable.

The fragment shader should also have access to the texture object, but how do we pass the texture object to the fragment shader? GLSL has a built-in data-type for texture objects called a *sampler* that takes as a postfix the texture type we want e.g. `sampler1D`, `sampler3D` or in our case `sampler2D`. We can then add a texture to the fragment shader by simply declaring a uniform `sampler2D` that we later assign our texture to.

    #version 330 core
    out vec4 FragColor;
    
    in vec3 ourColor;
    in vec2 TexCoord;

    uniform sampler2D ourTexture;

    void main()
    {
        FragColor = texture(ourTexture, TexCoord);
    }

To sample the color of a texture we use GLSL's built-in texture function that takes as its first argument a texture sampler and as its second argument the corresponding texture coordinates. The texture function then samples the corresponding color value using the texture parameters we set earlier. The output of this fragment shader is then the (filtered) color of the texture at the (interpolated) texture coordinate.

All that's left to do now is to bind the texture before calling `glDrawElements` and it will then automatically assign the texture to the fragment shader's sampler:

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

To get a little funky we can also mix the resulting texture color with the vertex colors. We simply multiply the resulting texture color with the vertex color in the fragment shader to mix both colors:

    FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);

#### Texture Units
You probably wondered why the `sampler2D` variable is a uniform if we didn't even assign it some value with glUniform. Using `glUniform1i` we can actually assign a location value to the texture sampler so we can set multiple textures at once in a fragment shader. This location of a texture is more commonly known as a *texture unit*. The _default texture unit for a texture is 0_ which is the default active texture unit so we didn't need to assign a location in the previous section; note that not all graphics drivers assign a default texture unit so the previous section may not have rendered for you.

_The main purpose of texture units is to allow us to use more than 1 texture in our shaders_. By assigning texture units to the samplers, we can bind to multiple textures at once as long as we activate the corresponding texture unit first. Just like glBindTexture we can activate texture units using `glActiveTexture` passing in the texture unit we'd like to use:

    glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
    glBindTexture(GL_TEXTURE_2D, texture);

After activating a texture unit, a subsequent `glBindTexture` call will bind that texture to the currently active texture unit. Texture unit `GL\_TEXTURE0` is always by default activated, so we didn't have to activate any texture units in the previous example when using `glBindTexture`.

We still however need to edit the fragment shader to accept another sampler. This should be relatively straightforward now:

    #version 330 core
    ...
    
    uniform sampler2D texture1;
    uniform sampler2D texture2;
    
    void main()
    {
        FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
    }

The final output color is now the combination of two texture lookups. GLSL's built-in mix function takes two values as input and linearly interpolates between them based on its third argument. If the third value is 0.0 it returns the first input; if it's 1.0 it returns the second input value. A value of 0.2 will return 80% of the first input color and 20% of the second input color, resulting in a mixture of both our textures.
