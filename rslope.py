def rslope(x,y,window):
    """
    Takes a data vector and a window to produce a vector of the running average slope.
    The window specifies the number of points on either side of the central point, so
    the total number of points in the slope fitting is 2*window+1.  Fitting is 
    done by the least squares method where the slope is defined by the equation below.
    the beginning and ends are padded with NaN, so fewer points are in those slope 
    estimates.  Addition and subtraction to the totals is used so that the sum is not
    recomputed each time, speeding the process.
    
                    sum(x)*sum(y)
        Sum(x*y) -  -------------
                          n
    m = -------------------------
                     (sum(x))^2
        sum(x^2) - --------------
                          n
    """
    
    import numpy as np
    
    # Check that x and y are the same length
    if len(x) != len(y): 
        print "Error: x and y must be the same length"
        return 0     
    
    N = len(x) # Number of points in the dataset
    slopes = np.ones(N) # Make array for slopes
    
    # Pad data with window number of points NaN on either side
    x_padded = np.empty(2*window+N)
    x_padded[0:window] = 0
    x_padded[window:N+window] = x
    x_padded[N+window:2*window+N] = 0
    
    y_padded = np.empty(2*window+N)
    y_padded[0:window] = 0
    y_padded[window:N+window] = y
    y_padded[N+window:2*window+N] = 0
    
    sum_x    = np.sum(x_padded[0:2*window+1])
    sum_y    = np.sum(y_padded[0:2*window+1])
    sum_x_sq = np.sum(x_padded[0:2*window+1]*x_padded[0:2*window+1])
    sum_xy   = np.sum(x_padded[0:2*window+1]*y_padded[0:2*window+1])

    n = np.empty(N)
    n[0:window] = np.arange(window+1,2*window+1)
    n[window:N-window] = window*2+1
    n[N-window:N] = np.arange(2*window,window,-1)
    
    slopes[0] = (sum_xy - (sum_x*sum_y/n[0]))/(sum_x_sq - (sum_x*sum_x/n[0]))
    
    for i in range(1,N):
        sum_x    = sum_x - x_padded[i-1] + x_padded[2*window+i]
        sum_y    = sum_y - y_padded[i-1] + y_padded[2*window+i]
        sum_x_sq = sum_x_sq - x_padded[i-1]*x_padded[i-1] + \
            x_padded[2*window+i]*x_padded[2*window+i]
        sum_xy   = sum_xy - x_padded[i-1]*y_padded[i-1] +\
            x_padded[2*window+i]*y_padded[2*window+i]
        slopes[i] = (sum_xy - (sum_x*sum_y/n[i]))/(sum_x_sq - (sum_x*sum_x/n[i]))
    return slopes

if __name__ == "__main__":    
    """
    If this is called as the main program, output the derivative to the screen.
    """
    import sys
    import numpy as np
    
    if len(sys.argv) != 2:
        print "ERROR: Include a file name for the program to run on, or import"
        print "it as a module. \n"
        print "Useage:"
        print "python rslope.py filename.txt OR TO SAVE python rslope.py filename.txt > output.txt"
        print "In a script: from rslope import *"
        sys.exit(0)
    
    infile = sys.argv[1]

    try:
        x,y = np.loadtxt(infile, unpack=True)
    except:
        print "Error in file format! Two columns of numbers only! Space delimited for x y."
        sys.exit(0)
        
    window = input('Enter window length (number of points on each side of datapoint to be used): ')
    derv = rslope(x,y,window)
    
    # Output to screen
    for item in zip(x,y):
        print "%f %f"%(item[0],item[1])