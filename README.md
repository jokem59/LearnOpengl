# LearnOpengl

## Notes
  - Windows x64 compiled GLFW library and include files are located in this repo because I'm working on multiple machines
  - GLAD utility: https://glad.dav1d.de/
  - Cmake command on Windows x64 using Visual Studio 2017
    - `cmake -G "Visual Studio 15 2017 Win64" ..`
    - Other options:
        Visual Studio 16 2019        = Generates Visual Studio 2019 project files.
                                       Use -A option to specify architecture.
        * Visual Studio 15 2017 [arch] = Generates Visual Studio 2017 project files.
                                         Optional [arch] can be "Win64" or "ARM".
          Visual Studio 14 2015 [arch] = Generates Visual Studio 2015 project files.
                                         Optional [arch] can be "Win64" or "ARM".
          Visual Studio 12 2013 [arch] = Generates Visual Studio 2013 project files.
                                         Optional [arch] can be "Win64" or "ARM".
          Visual Studio 11 2012 [arch] = Generates Visual Studio 2012 project files.
                                         Optional [arch] can be "Win64" or "ARM".
          Visual Studio 10 2010 [arch] = Generates Visual Studio 2010 project files.
                                         Optional [arch] can be "Win64" or "IA64".
          Visual Studio 9 2008 [arch]  = Generates Visual Studio 2008 project files.
                                         Optional [arch] can be "Win64" or "IA64".

## OpenGL Notes
### Vertex Buffer Objects - Managing GPU Memory
When passing VertexData to the GPU, this data gets stored in memory in the GPU and this area of memory is managed via the Vertex Buffer Object (VBO).  This is nice because the pipeline from CPU/Memory to GPU is relatively slow.  So by storing our VertexData in GPU memory, retrieval operations are very fast.  After binding a VBO with a certain buffer (e.g. `GL_BUFFER_ARRAY`), subsequent calls to `GL_BUFFER_ARRAY` are associated with the bound VBO.

The Vertex Shader lets us speciy any input in the form of vertex attributes.  Though flexible, this means we must manually specify what part of our input data (VectorData) goes to which vertex attribute.  The more attributes you have, the more times you need to specify these attributes anytime you want to draw your object.  To "save" attribute state, we can use Vector Array Objects (VAO).

Some examples of how Vector Data can vary:
VBO1: Position Only

    | pos[0] | pos[1] | pos[2] |

VBO2: Position and Color

    | pos[0] | col[0] | pos[1] | col[1] |

### Vertex Array Objects - Managing VBO and their Attributes
VAOs can be bound just like VBOs, and any subsequent vertex attribute calls from then on will be stored in the VAO.  The advantage with VAO is when configuring vertex attribute pointers, you only have to make the calls once and whenever we want to draw an object, just bind the corresponding VAO.  All the state is stored inside the VAO.

### Element Buffer Objects - Like a VBO but pass in indices to draw point in specified order
While VBO store a set of vertices, you can use an Element Buffer Object (EBO) to store a set of unique points, and then provide an array of indices to indicate the order you want to drawy our points in.  NOTE: OpenGL works in triangles so points and indices are specified in sets of three.  EBOs are bound to `GL_ELEMENT_ARRAY_BUFFER`.  Luckily VAO keep track of EBO.  The last EBO buffer that's bound to a VAO is stored as the VAOs Element Buffer Object.  Binding a VAO automatically the corresponding EBO.  If you want your VAO to keep track of your EBO, unbind the VAO first followed by the EBO.  If you unbind the EBO first, it will unbind your EBO from the VAO.

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
All shaders can specify *in* and *out* variables.  Specifically, an *in* variable is a *vertex attribute*, typically max of 16 but can be queried with `GL_MAX_VERTEX_ATTRIBUTE`.  You _link_ variables from one shader to another by specifying the *out* variable in one shader and specifying the same variable name as *in* in the subsequent shader.

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
  - `GL_REPEAT`: The default behavior for textures. Repeats the texture image.
  - `GL_MIRRORED_REPEAT`: Same as GL\_REPEAT but mirrors the image with each repeat.
  - `GL_CLAMP_TO_EDGE`: Clamps the coordinates between 0 and 1. The result is that higher coordinates become clamped to the edge, resulting in a stretched edge pattern.
  - `GL_CLAMP_TO_BORDER`: Coordinates outside the range are now given a user-specified border color.

Each of the aforementioned options can be set per coordinate axis (s, t (and r if you're using 3D textures) equivalent to x,y,z) with the glTexParameter* function:

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

The first argument specifies the texture target; we're working with 2D textures so the texture target is `GL_TEXTURE_2D`. The second argument requires us to tell what option we want to set and for which texture axis; we want to configure it for both the S and T axis. The last argument requires us to pass in the texture wrapping mode we'd like and in this case OpenGL will set its texture wrapping option on the currently active texture with GL\_MIRRORED\_REPEAT.

If we choose the `GL_CLAMP_TO_BORDER` option we should also specify a border color. This is done using the fv equivalent of the glTexParameter function with `GL_TEXTURE_BORDER_COLOR` as its option where we pass in a float array of the border's color value:

    float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

#### Texture Filtering
`GL_NEAREST` (also known as nearest neighbor or point filtering) is the default texture filtering method of OpenGL. When set to GL_NEAREST, OpenGL selects the texel that center is closest to the texture coordinate.

`GL_LINEAR` (also known as (bi)linear filtering) takes an interpolated value from the texture coordinate's neighboring texels, approximating a color between the texels. The smaller the distance from the texture coordinate to a texel's center, the more that texel's color contributes to the sampled color.

`GL_NEAREST` results in blocked patterns where we can clearly see the pixels that form the texture while `GL_LINEAR` produces a smoother pattern where the individual pixels are less visible. `GL_LINEAR` produces a more realistic output, but some developers prefer a more 8-bit look and as a result pick the `GL_NEAREST` option.

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#### Mipmaps
Imagine we had a large room with thousands of objects, each with an attached texture. There will be objects far away that have the same high resolution texture attached as the objects close to the viewer. Since the objects are far away and probably only produce a few fragments, OpenGL has difficulties retrieving the right color value for its fragment from the high resolution texture, since it has to pick a texture color for a fragment that spans a large part of the texture. This will produce visible artifacts on small objects, not to mention the waste of memory bandwidth using high resolution textures on small objects.

To solve this issue OpenGL uses a concept called *mipmaps* that is basically a collection of texture images where each subsequent texture is twice as small compared to the previous one. The idea behind mipmaps should be easy to understand: after a certain distance threshold from the viewer, OpenGL will use a different mipmap texture that best suits the distance to the object. Because the object is far away, the smaller resolution will not be noticeable to the user. OpenGL is then able to sample the correct texels, and there's less cache memory involved when sampling that part of the mipmaps.

Creating a collection of mipmapped textures for each texture image is cumbersome to do manually, but luckily OpenGL is able to do all the work for us with a single call to `glGenerateMipmaps` after we've created a texture.

When switching between mipmaps levels during rendering OpenGL may show some artifacts like sharp edges visible between the two mipmap layers. Just like normal texture filtering, it is also possible to filter between mipmap levels using NEAREST and LINEAR filtering for switching between mipmap levels. To specify the filtering method between mipmap levels we can replace the original filtering methods with one of the following four options:

  - `GL_NEAREST_MIPMAP_NEAREST`: takes the nearest mipmap to match the pixel size and uses nearest neighbor interpolation for texture sampling.
  - `GL_LINEAR_MIPMAP_NEAREST`: takes the nearest mipmap level and samples that level using linear interpolation.
  - `GL_NEAREST_MIPMAP_LINEAR`: linearly interpolates between the two mipmaps that most closely match the size of a pixel and samples the interpolated level via nearest neighbor interpolation.
  - `GL_LINEAR_MIPMAP_LINEAR`: linearly interpolates between the two closest mipmaps and samples the interpolated level via linear interpolation.

Just like texture filtering we can set the filtering method to one of the 4 aforementioned methods using `glTexParameteri`:

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#### Loading and Creating Textures
Use an image-loading library that supports several popular formats and does all the hard work for us. A library like `stb_image.h`.

By defining `STB_IMAGE_IMPLEMENTATION` the preprocessor modifies the header file such that it only contains the relevant definition source code, effectively turning the header file into a .cpp file, and that's about it. Now simply include stb_image.h somewhere in your program and compile.

To load an image using `stb_image.h` we use its `stbi_load` function:

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
  - The first argument specifies the texture target; setting this to `GL_TEXTURE_2D` means this operation will generate a texture on the currently bound texture object at the same target (so any textures bound to targets `GL_TEXTURE_1D` or `GL_TEXTURE_3D` will not be affected).
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

Note that we have to adjust the stride parameter of the previous two vertex attributes to `8 * sizeof(float)` as well.

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

After activating a texture unit, a subsequent `glBindTexture` call will bind that texture to the currently active texture unit. Texture unit `GL_TEXTURE0` is always by default activated, so we didn't have to activate any texture units in the previous example when using `glBindTexture`.

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

We now want to load and create another texture; you should be familiar with the steps now. Make sure to create another texture object, load the image and generate the final texture using `glTexImage2D`. For the second texture we'll use an image of your facial expression while learning OpenGL:

    unsigned char *data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

Note that we now load a .png image that includes an alpha (transparency) channel. This means we now need to specify that the image data contains an alpha channel as well by using `GL_RGBA`; otherwise OpenGL will incorrectly interpret the image data.

To use the second texture (and the first texture) we'd have to change the rendering procedure a bit by binding both textures to the corresponding *texture unit*:

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

We also have to tell OpenGL to which texture unit each shader sampler belongs to by setting each sampler using `glUniform1i`. We only have to set this once, so we can do this before we enter the render loop:

    ourShader.use(); // don't forget to activate the shader before setting uniforms!  
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // set it manually
    ourShader.setInt("texture2", 1); // or with shader class
    
    while(...)
    {
        [...]
    }

We can flip the second texture along across the y-axis by multiplying the texture's x-coord by -1.0.  The function `texture`, for a `sampler2D` data type, takes a `vec2` and we can using *swizzling* to change which coordinate of the Texture is mapped to the vertex coordinate.  Because we originally specified the texture coordinates [0.0, 1.0] to match the vetex data position coordinates, with no changes, we see the image as expected.  If we multiply the x-coord of the texture coordinate provide by -1.0, the expectation is to get a mirrored effect across the y-axis.

    #version 330 core
    out vec4 FragColor;
    
    in vec3 ourColor;
    in vec2 TexCoord;
    
    uniform sampler2D texture1;
    uniform sampler2D texture2;
    
    void main()
    {
        FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(TexCoord.x * -1.0, TexCoord.y)), 0.2);
    }


### Transformations
We now know how to create objects, color them and/or give them a detailed appearance using textures, but they're still not that interesting since they're all static objects. We could try and make them move by changing their vertices and re-configuring their buffers each frame, but that's cumbersome and costs quite some processing power. There are much better ways to *transform* an object and that's by using (multiple) matrix objects.

#### Vectors
*Dot Product* - The dot product of two vectors is equal to the scalar product of their lengths times the cosine of the angle between them.  The dot product *only* defines the angle between both vectors.

*Cross Product* - The cross product is only defined in 3D space and takes two non-parallel vectors as input and produces a third vector that is orthogonal to both the input vectors. If both the input vectors are orthogonal to each other as well, a cross product would result in 3 orthogonal vectors

#### Matrices
A matrix is a rectangular array of numbers, symbols and/or mathematical expressions. Each individual item in a matrix is called an element of the matrix. An example of a 2x3 matrix is shown below:

    [ 1 2 3 ]
    [ 4 5 6 ]

Matrices are indexed by `(i,j)` where `i` is the row and `j` is the column, that is why the above matrix is called a 2x3 matrix (3 columns and 2 rows).  To retrieve the value `4` we would index it as `(2,1)` (second row, first column).

Matrix multiplication basically means to follow a set of pre-defined rules when multiplying. There are a few restrictions though:

  - You can only multiply two matrices if the number of columns on the left-hand side matrix is equal to the number of rows on the right-hand side matrix.
  - Matrix multiplication is not commutative that is `A⋅B≠B⋅A`.

#### Matrix-Vector Multiplication

Matrix multiplication happens from RIGHT to LEFT
*Identity Matrix* - 4x4 matrix with 1's across the diaganol.  When multiplied with a four element vector (a 4x1 matrix), this transformation does not affect the vector.

    [ 1 0 0 0 ]   [ 1 ]   [ 1 * 1 ]   [ 1 ]
    [ 0 1 0 0 ] * [ 2 ] = [ 1 * 2 ] = [ 2 ]
    [ 0 0 1 0 ]   [ 3 ]   [ 1 * 3 ]   [ 3 ]
    [ 0 0 0 1 ]   [ 4 ]   [ 1 * 4 ]   [ 4 ]

You may be wondering what the use is of a transformation matrix that does not transform? The identity matrix is usually a starting point for generating other transformation matrices and if we dig even deeper into linear algebra, a very useful matrix for proving theorems and solving linear equations.

*Translation* - starting with identity matrix or any transformed identity matrix, move a vector to a new position.

    [ 1 0 0 Tx ]   [ x ]   [ x + Tx ]
    [ 0 1 0 Ty ] * [ y ] = [ y + Ty ]
    [ 0 0 1 Tz ]   [ z ]   [ z + Tz ]
    [ 0 0 0 1  ]   [ 1 ]   [   1    ]

This works because all of the translation values are multiplied by the vector's w column and added to the vector's original values.  With a translation matrix we can move objects in any of the 3 axis directions (x, y, z), making it a very useful transformation matrix for our transformation toolkit.

*Scale* - starting with identity matrix or any transformed identity matrix, scale each vector element accordingly.

    [ S1 0  0  0 ]   [ x ]   [ S1 * x ]
    [ 0  S2 0  0 ] * [ y ] = [ S2 * y ]
    [ 0  0  S3 0 ]   [ z ]   [ S3 * z ]
    [ 0  0  0  1 ]   [ 1 ]   [    1   ]

Note that we keep the 4th scaling value 1. The w component is used for other purposes as we'll see later on.

*Rotate* - starting with identity matrix or any transformed identity matrix, rotate the vector elements X degrees/radians.

First let's define what a rotation of a vector actually is. A rotation in 2D or 3D is represented with an angle. An angle could be in degrees or radians where a whole circle has 360 degrees or 2 PI radians. I prefer explaining rotations using degrees as we're generally more accustomed to them.

Rotations in 3D are specified with an angle and a rotation axis. The angle specified will rotate the object along the rotation axis given. Try to visualize this by spinning your head a certain degree while continually looking down a single rotation axis. When rotating 2D vectors in a 3D world for example, we set the rotation axis to the z-axis (try to visualize this).

#### Combining Matrices

Order of Operations: Scale -> Rotation -> Translation.  Breaking this order can have negative effects on your other transformations (e.g. your rotation will get scaled as well).
  - In matrix multiplation, this order of operation looks like:
    - [TRANSLATION] * [ROTATION] * [SCALE] = [TRANSFORMATION]
    - [TRANSFORMATION] * [VECTOR] => Transformed Vector

### Coordinate Systems

In the last chapter we learned how we can use matrices to our advantage by transforming all vertices with transformation matrices. OpenGL expects all the vertices, that we want to become visible, to be in normalized device coordinates after each vertex shader run. That is, the x, y and z coordinates of each vertex should be between -1.0 and 1.0; coordinates outside this range will not be visible. What we usually do, is specify the coordinates in a range (or space) we determine ourselves and in the vertex shader transform these coordinates to normalized device coordinates (NDC). These NDC are then given to the rasterizer to transform them to 2D coordinates/pixels on your screen.

Transforming coordinates to NDC is usually accomplished in a step-by-step fashion where we transform an object's vertices to several coordinate systems before finally transforming them to NDC. The advantage of transforming them to several intermediate coordinate systems is that some operations/calculations are easier in certain coordinate systems as will soon become apparent. There are a total of 5 different coordinate systems that are of importance to us:

  - Local space (or Object space)
  - World space
  - View space (or Eye space)
  - Clip space
  - Screen space

To transform the coordinates from one space to the next coordinate space we'll use several transformation matrices of which the most important are the *model*, *view* and *projection matrix*. Our vertex coordinates first start in *local space* as local coordinates and are then further processed to *world coordinates*, *view coordinates*, *clip coordinates* and eventually end up as *screen coordinates*.

  1. Local coordinates are the coordinates of your object relative to its local origin; they're the coordinates your object begins in.
  2. The next step is to transform the local coordinates to world-space coordinates which are coordinates in respect of a larger world. These coordinates are relative to some global origin of the world, together with many other objects also placed relative to this world's origin.
  3. Next we transform the world coordinates to view-space coordinates in such a way that each coordinate is as seen from the camera or viewer's point of view.
  4. After the coordinates are in view space we want to project them to clip coordinates. Clip coordinates are processed to the -1.0 and 1.0 range and determine which vertices will end up on the screen. Projection to clip-space coordinates can add perspective if using perspective projection.
  5. And lastly we transform the clip coordinates to screen coordinates in a process we call viewport transform that transforms the coordinates from -1.0 and 1.0 to the coordinate range defined by glViewport. The resulting coordinates are then sent to the rasterizer to turn them into fragments.

You probably got a slight idea what each individual space is used for. The reason we're transforming our vertices into all these different spaces is that some operations make more sense or are easier to use in certain coordinate systems. For example, when modifying your object it makes most sense to do this in local space, while calculating certain operations on the object with respect to the position of other objects makes most sense in world coordinates and so on. If we want, we could define one transformation matrix that goes from local space to clip space all in one go, but that leaves us with less flexibility.
"

#### Local Space
Local space is the coordinate space that is local to your object, i.e. where your object begins in. Imagine that you've created your cube in a modeling software package (like Blender). The origin of your cube is probably at (0,0,0) even though your cube may end up at a different location in your final application. Probably all the models you've created all have (0,0,0) as their initial position. All the vertices of your model are therefore in local space: they are all local to your object.

The vertices of the container we've been using were specified as coordinates between -0.5 and 0.5 with 0.0 as its origin. These are local coordinates.

#### World Space
If we would import all our objects directly in the application they would probably all be somewhere positioned inside each other at the world's origin of (0,0,0) which is not what we want. We want to define a position for each object to position them inside a larger world. The coordinates in world space are exactly what they sound like: the coordinates of all your vertices relative to a (game) world. This is the coordinate space where you want your objects transformed to in such a way that they're all scattered around the place (preferably in a realistic fashion). The coordinates of your object are transformed from local to world space; this is accomplished with the model matrix.

The model matrix is a transformation matrix that translates, scales and/or rotates your object to place it in the world at a location/orientation they belong to. Think of it as transforming a house by scaling it down (it was a bit too large in local space), translating it to a suburbia town and rotating it a bit to the left on the y-axis so that it neatly fits with the neighboring houses. You could think of the matrix in the previous chapter to position the container all over the scene as a sort of model matrix as well; we transformed the local coordinates of the container to some different place in the scene/world.

#### View Space
NOTE: When moving the View Matrix, 5.0f in the x-axis means you're moving the WHOLE WORLD 5.0 units in the positive x-direction.  This does n't mean you're moving the View/Camera.

The view space is what people usually refer to as the camera of OpenGL (it is sometimes also known as camera space or eye space). The view space is the result of transforming your world-space coordinates to coordinates that are in front of the user's view. The view space is thus the space as seen from the camera's point of view. This is usually accomplished with a combination of translations and rotations to translate/rotate the scene so that certain items are transformed to the front of the camera. These combined transformations are generally stored inside a view matrix that transforms world coordinates to view space. In the next chapter we'll extensively discuss how to create such a view matrix to simulate a camera.

#### Clip Space
At the end of each vertex shader run, OpenGL expects the coordinates to be within a specific range and any coordinate that falls outside this range is clipped. Coordinates that are clipped are discarded, so the remaining coordinates will end up as fragments visible on your screen. This is also where clip space gets its name from.

Because specifying all the visible coordinates to be within the range -1.0 and 1.0 isn't really intuitive, we specify our own coordinate set to work in and convert those back to NDC as OpenGL expects them.

To transform vertex coordinates from view to clip-space we define a so called *projection matrix* that specifies a range of coordinates e.g. -1000 and 1000 in each dimension. The projection matrix then transforms coordinates within this specified range to normalized device coordinates (-1.0, 1.0). All coordinates outside this range will not be mapped between -1.0 and 1.0 and therefore be clipped. With this range we specified in the projection matrix, a coordinate of (1250, 500, 750) would not be visible, since the x coordinate is out of range and thus gets converted to a coordinate higher than 1.0 in NDC and is therefore clipped.

Note that if only a part of a primitive e.g. a triangle is outside the clipping volume OpenGL will reconstruct the triangle as one or more triangles to fit inside the clipping range.

This *viewing box* a projection matrix creates is called a *frustum* and each coordinate that ends up inside this frustum will end up on the user's screen. The total process to convert coordinates within a specified range to NDC that can easily be mapped to 2D view-space coordinates is called *projection* since the projection matrix projects 3D coordinates to the easy-to-map-to-2D normalized device coordinates.

Once all the vertices are transformed to clip space a final operation called *perspective division* is performed where we divide the `x`, `y` and `z` components of the position vectors by the vector's homogeneous `w` component; perspective division is what transforms the 4D clip space coordinates to 3D normalized device coordinates. This step is performed automatically at the end of the vertex shader step.

It is after this stage where the resulting coordinates are mapped to screen coordinates (using the settings of `glViewport`) and turned into fragments.

The projection matrix to transform view coordinates to clip coordinates usually takes two different forms, where each form defines its own unique frustum. We can either create an `orthographic projection matrix` or a `perspective projection matrix`.

#### Orthographic Projection
An orthographic projection matrix defines a cube-like frustum box that defines the clipping space where each vertex outside this box is clipped. When creating an orthographic projection matrix we specify the width, height and length of the visible frustum. All the coordinates inside this frustum will end up within the NDC range after transformed by its matrix and thus won't be clipped.

The frustum defines the visible coordinates and is specified by a width, a height and a near and far plane. Any coordinate in front of the near plane is clipped and the same applies to coordinates behind the far plane. The orthographic frustum directly maps all coordinates inside the frustum to normalized device coordinates without any special side effects since it won't touch the `w` component of the transformed vector; if the `w` component remains equal to 1.0 perspective division won't change the coordinates.

To create an orthographic projection matrix we make use of GLM's built-in function `glm::ortho`:

    glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);

The first two parameters specify the left and right coordinate of the frustum and the third and fourth parameter specify the bottom and top part of the frustum. With those 4 points we've defined the size of the near and far planes and the 5th and 6th parameter then define the distances between the near and far plane. *This specific projection matrix transforms all coordinates between these `x`, `y` and `z` range values to normalized device coordinates*.

An orthographic projection matrix directly maps coordinates to the 2D plane that is your screen, but in reality a direct projection produces unrealistic results since the projection doesn't take perspective into account. That is something the perspective projection matrix fixes for us.

#### Perspective Projection
If you ever were to enjoy the graphics the real life has to offer you'll notice that objects that are farther away appear much smaller. This weird effect is something we call perspective.

As you can see, due to perspective the lines seem to coincide at a far enough distance. *This is exactly the effect perspective projection tries to mimic and it does so using a perspective projection matrix.* The projection matrix maps a given frustum range to clip space, but also manipulates the `w` value of each vertex coordinate in such a way that _the further away a vertex coordinate is from the viewer, the higher this w component becomes._ Once the coordinates are transformed to clip space they are in the range `-w` to `w` (anything outside this range is clipped). OpenGL requires that the visible coordinates fall between the range -1.0 and 1.0 as the final vertex shader output, thus once the coordinates are in clip space, perspective division is applied to the clip space coordinates:

    out = [ x/w ]
          [ y/w ]
          [ z/w ]

Each component of the vertex coordinate is divided by its `w` component giving smaller vertex coordinates the further away a vertex is from the viewer. This is another reason why the `w` component is important, since it helps us with perspective projection. The resulting coordinates are then in normalized device space.

A perspective projection matrix can be created in GLM as follows:

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f))

What `glm::perspective` does is again create a large frustum that defines the visible space, anything outside the frustum will not end up in the clip space volume and will thus become clipped. A perspective frustum can be visualized as a non-uniformly shaped box from where each coordinate inside this box will be mapped to a point in clip space.

Its first parameter defines the `fov` value, that stands for *field of view* and sets how large the viewspace is. For a realistic view it is usually set to 45 degrees, but for more doom-style results you could set it to a higher value. The second parameter sets the aspect ratio which is calculated by dividing the viewport's width by its height. The third and fourth parameter set the near and far plane of the frustum. We usually set the near distance to 0.1 and the far distance to 100.0. All the vertices between the near and far plane and inside the frustum will be rendered.

Whenever the near value of your perspective matrix is set too high (like 10.0), OpenGL will clip all coordinates close to the camera (between 0.0 and 10.0), which can give a visual result you maybe have seen before in videogames where you could see through certain objects when moving uncomfortably close to them.

When using *orthographic projection*, each of the vertex coordinates are directly mapped to clip space without any fancy perspective division (it still does perspective division, but the w component is not manipulated (it stays 1) and thus has no effect). Because the orthographic projection doesn't use perspective projection, objects farther away do not seem smaller, which produces a weird visual output. _For this reason the orthographic projection is mainly used for 2D renderings and for some architectural or engineering applications where we'd rather not have vertices distorted by perspective._ Applications like Blender that are used for 3D modeling sometimes use orthographic projection for modeling, because it more accurately depicts each object's dimensions.

You can see that with perspective projection, the vertices farther away appear much smaller, while in orthographic projection each vertex has the same distance to the user.

#### Putting it all together
We create a transformation matrix for each of the aforementioned steps: model, view and projection matrix. A vertex coordinate is then transformed to clip coordinates as follows:

    Vclip = Mprojection * Mview * Mmodel * Vlocal

Note: Matrix multiplication order is right to left so order is: Local Matrix -> Model Matrix -> View Matrix -> Projection Matrix.  The resulting vertex should then be assigned to `gl_Position` in the vertex shader and OpenGL will then automatically perform perspective division and clipping.

The output of the vertex shader requires the coordinates to be in clip-space which is what we just did with the transformation matrices. OpenGL then performs perspective division on the clip-space coordinates to transform them to normalized-device coordinates. OpenGL then uses the parameters from glViewPort to map the normalized-device coordinates to screen coordinates where each coordinate corresponds to a point on your screen (in our case a 800x600 screen). This process is called the *viewport transform*.

#### Going 3D
Now that we know how to transform 3D coordinates to 2D coordinates we can start rendering real 3D objects instead of the lame 2D plane we've been showing so far.

To start drawing in 3D we'll first create a model matrix. The model matrix consists of translations, scaling and/or rotations we'd like to apply to transform all object's vertices to the global world space. Let's transform our plane a bit by rotating it on the x-axis so it looks like it's laying on the floor. The model matrix then looks like this:

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 

*By multiplying the vertex coordinates with this model matrix we're transforming the vertex coordinates to world coordinates.* Our plane that is slightly on the floor thus represents the plane in the global world.

Next we need to create a view matrix. We want to move slightly backwards in the scene so the object becomes visible (when in world space we're located at the origin (0,0,0)). To move around the scene, think about the following:

  - To move a camera backwards, is the same as moving the entire scene forward.
  - That is exactly what a view matrix does, we move the entire scene around inversed to where we want the camera to move.
  - Because we want to move backwards and since OpenGL is a right-handed system we have to move in the positive z-axis. We do this by translating the scene towards the negative z-axis. This gives the impression that we are moving backwards.

We'll discuss how to move around the scene in more detail in the next chapter. For now the view matrix looks like this:

    glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

The last thing we need to define is the projection matrix. We want to use perspective projection for our scene so we'll declare the projection matrix like this:

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

Now that we created the transformation matrices we should pass them to our shaders. First let's declare the transformation matrices as uniforms in the vertex shader and multiply them with the vertex coordinates:

    #version 330 core
    layout (location = 0) in vec3 aPos;
    ...
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        // note that we read the multiplication from right to left
        gl_Position = projection * view * model * vec4(aPos, 1.0);
        ...
    }

We should also send the matrices to the shader (this is usually done each frame since transformation matrices tend to change a lot):

    int modelLoc = glGetUniformLocation(ourShader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    ... // same for View Matrix and Projection Matrix

Now that our vertex coordinates are transformed via the model, view and projection matrix the final object should be:

  - Tilted backwards to the floor.
  - A bit farther away from us.
  - Be displayed with perspective (it should get smaller, the further its vertices are).

#### More 3d


    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
