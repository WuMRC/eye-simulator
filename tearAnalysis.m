%% Single threshold

image = imadjust(rgb2gray(imread('tearThing.png')));

% figure, imshow(image)
thresh = graythresh(image);

levelPercent = 0.1:0.1:2;

figure('units','normalized','outerposition',[0 0 1 1])
for indImage = 1:length(levelPercent)
    imageBW = im2bw(image,thresh*levelPercent(indImage));
    subplot(4,5,indImage), imshow(imageBW)
    title(strcat(num2str(levelPercent(indImage)*100),'% of graythresh'))
end


%% Multithreshold

threshLevel = 1:12;
figure('units','normalized','outerposition',[0 0 1 1])
for indThresh = 1:length(threshLevel)
    thresh = multithresh(image,threshLevel(indThresh));
    Iseg = imquantize(image,thresh);
    RGB = label2rgb(Iseg);
    subplot(3,4,indThresh), imshow(RGB)
    title(strcat(num2str(indThresh),' threshold levels'))
end


%% Find meniscus
image = imadjust(rgb2gray(imread('tearThing.png')));
thresh = graythresh(image);
imageBW = im2bw(image,thresh*0.31);
% imshow(imageBW)
[nRow, nCols] = size(image);
for indCol = 1:nCols
    meniscus(indCol) = find(imageBW(:,indCol),1,'first');   
end

meniscus(meniscus==1)= NaN;

figure, imshow(image), hold on
plot(meniscus,'Color','w','LineWidth',2)

%% Select ROI, find walls
% Select region of interest
clear imageROI RGB Iseg wall

image = imadjust(rgb2gray(imread('tearThing.png')));
figure, imshow(image)
hBox = imrect;
roiPosition = wait(hBox);

% roiPosition;
roi_xind = round([roiPosition(2), roiPosition(2), ...
    roiPosition(2)+roiPosition(4), roiPosition(2)+roiPosition(4)]);
roi_yind = round([roiPosition(1), roiPosition(1)+roiPosition(3), ...
    roiPosition(1)+roiPosition(3), roiPosition(1)]);
close

imageROI = image(roi_xind(1):roi_xind(3),roi_yind(1):roi_yind(2));



% Multithreshold
threshLevel = 1:3;
figure('units','normalized','outerposition',[0 0 1 1])
subplot(2,2,1), imshow(imageROI)
for indThresh = 1:length(threshLevel)
    thresh = multithresh(imageROI,threshLevel(indThresh));
    Iseg = imquantize(imageROI,thresh);
    RGB(:,:,indThresh) = rgb2gray(label2rgb(Iseg));
    subplot(2,2,indThresh+1), imshow(RGB(:,:,indThresh))
    title(strcat(num2str(indThresh),' threshold levels'))
end


%%
[nRow, nCols, nLevels] = size(RGB);
for indCol = 1:nCols
    wall(indCol) = find(im2bw(RGB(:,indCol,2)),1,'first');
    wallAbs(:,indCol) = [roi_xind(2)+wall(indCol) roi_yind(1)+indCol-1];
end

wall(wall==1)=NaN;

wallAbs(1,wallAbs(1,:)==roi_xind(2)+1)=0;
wallAbs(2,wallAbs(1,:)==0)=0;
wallAbs(wallAbs==0)=NaN
% 
% 
% figure
% imshow(imageROI), hold on
% plot(wall,'Color','w','LineWidth',2)
% % ()




figure, imshow(image)
hold on, plot(wallAbs(2,1:end-1),wallAbs(1,1:end-1),...
    'Color','w','LineWidth',2)
hold on, plot(meniscus,'Color','y','LineWidth',2)