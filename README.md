# HUGE
HUGE (Highly Unoptimized Game Engine)  
Basically wanted to build some game, but ended up doing some weird pseudo game engine, which i don't want to continue developing  

# Install
Create "Include" folder and add boost, box2d, GL, GLFW, glm includes

# See
![](demo.gif)

# Issues
Body will be created all the time, even if checkbox is unchecked. (So when body is created and placed, it will have weird position).  
Need to set current context for threads and such to create body in different threads.
