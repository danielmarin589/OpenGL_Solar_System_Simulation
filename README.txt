PSA: In order to run both of the tasks you should open the .sln files. I tried running the .exes in the debug folders and they don't work.

Solar System Simulation

Firstly, in order to create a sphere in my project I used the Sphere class found on the website http://www.songho.ca/opengl/gl_sphere.html#sphere (the one that you gave us). I placed the class into a header and imported it into my project, calling functions from it in my main so as to create a sphere. Basically I'm using only one set of coordinates of sphere centered in (0,0,0), coordinates which I send to the buffer. What I actually do is, I render multiple spheres with coordinates (0,0,0) from the buffer and translate their position each time,so that I get 8 planets and a sun (9 spheres total). 

I defined a struct named cBody(from celestialBody) to store data(postion,orbiting speed, so on) about each planet + sun.

For the position of the sun I rendered a sphere in the center(0,0,0)(so that all the other planets revolve around it) with the radius of 10.0f (note: the Sun should have the biggest radius out of all bodies in the system).

For all the planets I set different values, relatively close to the actual planets, but on some parts such as distance I had to cut some of it since we are not astrologist working at NASA. After defining each parameter for each planet from the struct I pushed all the instaces of the struct(except the sun) into a vector called planets(very suggestive, I know).

In the while function(in main) I created the Sun using a newly defined mat4 model matrix, which I multiply it with the values of some parameters of the sun (defined earlier with the struct) in order to translate and scale it. 

This process is almost identical for all the 8 planets except for the fact that we have a for loop and the model matrix(previously defined) will take the values of the identity matrix for each start of the loop(we don't need two model matrices).

For rotation and translation I used two booleans (initially defined as false) named flagRot and flagTrans that are controlled by the user input and are checked in the while loop. I also defined two variables named elapsedTimeR and elapsedTimeR that I use to keep track of the planets time while the simulation is running. When I toggle the rotation/translation by pressing R/T, the elapsedTimeR/elapsedTimeT will be incremented with deltaTime.
By using these two timers, the simulation can be stopped and resumed(from the last stopping point, not reseted) at any given point. I also added a key(ESC) that resets the values for the timers and stops the flags. By doing this the simulation will reset. For all the user input I used key callbacks.

For the camera I added the one we created during the last lab.

For the colors I used a glm:vec4 vector and I passed it as a paramter for the struct cBody. In the for loop I sent the color vector for the currently drawn planets to the fragment shader using an uniform.


Functionalities checklist:
+Window Creation
+Camera integration
+PlanetsRendering(+Sun) + correct positions + colors 
+User Input
+Transforms (Rotation/Translation/Scale)

