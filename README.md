<h1 align="center">2D Particle-based Viscoelastic Fluid Simulation</h1>

# <p>Description</p>

<p>
This is a fluid Simulation based on: <br> <i>Simon Clavet, Philippe Beaudoin, and Pierre Poulin. Particle-based viscoelastic fuid simulation. In Proceedings of the 2005 ACM SIGGRAPH/Eurographics symposium on Computer animation, pages 219-228. ACM, 2005.</i>
<br>It is implemented using C++ 11 and rendered with OpenGL 4.3.
</p>

# <p>Usage</p>

<p>
In order to run the simulation you have to compile it yourself. For that, you also need to compile glfw and add it to a folder called "lib".
<br> Both Windows and Linux should work, provided all libraries for OpenGL are installed.

## <p1>Loading Maps</p1>
You can use Images in order to create custom maps
* A white pixels is counted as free space whereas a black pixels is counted as wall 
* Only .png files are supported.
* When loading a custom map for the first time a data file gets created so on subsequent uses loading is way faster.
* If you wish to have a different back-/ foreground on loaded Maps, add the images you wish to use and name them *filename*_bg.png / *filename*_fb.png respectively. 
</p>

# <p>Screenshots</p>

# <p>Notes</p>
<p>
This was supposed to be a small project but turned out to be a lot more complicated.
Therefore please excuse that the simulation breaks from time to time and is in no way perfect.
</p>