<h1 align="center">2D Particle-based Fluid Simulation</h1>

# <p>Description</p>

<p>
This is a fluid Simulation based on: <br> <i>Simon Clavet, Philippe Beaudoin, and Pierre Poulin. Particle-based viscoelastic fuid simulation. In Proceedings of the 2005 ACM SIGGRAPH/Eurographics symposium on Computer animation, 2005.</i>
<br>
<br>It is implemented using C++ 11 and rendered with OpenGL 4.3.
</p>

# <p>Usage</p>

<p>
A Windows build already exists, however feel free to build it yourself. Please mind that you might be running into problems with MSVC.
<br>For Linux, you have to build it yourself.
<br> Both Windows and Linux should work, provided all libraries for OpenGL are installed.

## <p1>Loading Maps</p1>
You can use Images in order to create custom maps
* A white pixels is counted as free space whereas a black pixels is counted as wall 
* Only .png files are supported.
* When loading a custom map for the first time a data file gets created so on subsequent uses loading is way faster.
* If you wish to have a different back-/ foreground on loaded Maps, add the images you wish to use and name them *filename*_bg.png / *filename*_fb.png respectively. 
</p>

# <p>Screenshots</p>
![BoxIdle](https://user-images.githubusercontent.com/72945679/218122916-c64c1c54-07f2-472d-b695-b9f299cd350b.png)
![BoxFlow](https://user-images.githubusercontent.com/72945679/218122854-d03d206f-a6de-4314-ba44-21ec9e318f92.png)
![Tube](https://user-images.githubusercontent.com/72945679/218122958-750262d8-d8e4-4b4a-8d1c-0983a238fc0d.png)
![slope](https://user-images.githubusercontent.com/72945679/218122976-be7b6e75-9116-4fd5-a504-b1382f234736.png)
![BoxGrid](https://user-images.githubusercontent.com/72945679/218123009-57d061c2-b5fb-4e42-b735-7bede2c3b27c.png)

#

## <p>Side Notes</p>
<p>
This was supposed to be a small project but turned out to be a lot more complicated.
Therefore please excuse the bugs and the performance issues.
</p>
