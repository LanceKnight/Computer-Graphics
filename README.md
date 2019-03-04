The maxium allowed nested read is 6. To change this setting, change the variable <numNested> in main.cpp somewhere below #include

you can execute:

**read resize.test**


**read border-kernal.test**

for testing of Resize, Zoom, Border, Select respectively

I implemented extra features including BORDER, SELECT commands. Resize can also handle negative scale factors.

# Efficiency explaination:
in the convolution, I didn't go over all pixels in the original image since most pixels are outside of the filter, making its value zero. So I set up a upper limit and lower limit for the k to loop though, which makes the program much faster.

# Comparison of results using different filter/border
## all tests below were using "resize -2 -2"
### tests of border. Box filter was used.
1. The border with 0 values has a 0 border will have black border, which is not very ideal (see results/zero_border_20.tif for window of 20, results/zero_border_3.tif for window of 3) 
2. The border with edge value extends the edge values, it is not very obseravle with small window size, but more salient with bigger window size (see results/freeze_border_20.tif for window of 20, results/freeze_border_3.tif for window of 3)
3. The border with mirror image value has the most natural result, thus the best by comparison with others (see results/circular_border_20.tif for window of 20, results/circular_border_3.tif for window of 3)
### tests of filter. Circular border was used. see results in results/filters 
1. mitchell seems to give the best result in terms of resolution
2. box filter seems to give grid-like result
3. triangle and lanczos both seem to improve the grid-like result over box filter
4. gaussian filter smoothes the image a lot, making it more blurry.
