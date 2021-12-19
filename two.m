myImage = imread('2.png');
intImage = myImage(:,:,1)/3+myImage(:,:,2)/3+myImage(:,:,3)/3;
imshow(intImage)
myNumber = classify(net,intImage)