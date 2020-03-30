#version 430

// https://www.reddit.com/r/Unity3D/comments/7pa6bq/drawing_mandelbrot_fractal_using_gpu_compute/

RWTexture2D<float4> textureOut;						// this is the texture we draw pixels into
RWStructuredBuffer<double> rect;					// these are boundaries in fractal space that are currently visible on the screen
RWStructuredBuffer<float4> colors;					// this is a set of colors that has been prepared on CPU side and been written to GPU

#pragma kernel pixelCalc							// kernel name declaration, we'll use the name to call kernel from CPU side
    uint itn = 0;
							// this directive defines the amount of threads this kernel will be runned in

void pixelCalc (uint3 id : SV_DispatchThreadID){	// now we write kernel's code. id parameter contains thread's index and used to access the right data
    float k = 0.0009765625;							// this is simply 1/1024, used to project 1024x1024 texture space to a 2x2 fractal space
    double dx, dy;
    double p, q;
    double x, y, xnew, ynew, d = 0;					// we use double precision variables, to avoid precision limit for a bit longer while going deeper in the fractal
    uint itn = 0;
    dx = rect[2] - rect[0];
    dy = rect[3] - rect[1];
    p = rect[0] + ((int)id.x) * k * dx;
    q = rect[1] + ((int)id.y) * k * dy;
    x = p;
    y = q;
    while (itn < 255 && d < 4){						// the essense of the fractal: in this loop we check how many steps it takes for a point to leave 2x2 fractal area
        xnew = x * x - y * y + p;
        ynew = 2 * x * y + q;
        x = xnew;
        y = ynew;
        d = x * x + y * y;
        itn++;
    }
    textureOut[id.xy] = colors[itn];				// this is how we write pixel's color: id parameter defines the pixel, and number of steps defines color
}