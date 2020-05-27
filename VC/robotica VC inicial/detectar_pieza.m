
im = imread("./piezas/ros.jpg");
im = rgb2gray(im);

kernel = ones(5,5);

figure(1),imshow(im);

% treshold i tractament de la imatge (nomes a un dataset)
im = im < 10;
im = imdilate(im, kernel);

% contes punts de tota la imatge
lm_label = bwlabel(im,8);
dots = max(max(lm_label));
figure(2),imshow(im);
%disp(dots);

% contes punts de la primera meitat de la imatge
n = size(im,2);
half_im = im(:,1:fix(n/2));

figure(3),imshow(half_im);
lm_label = bwlabel(half_im,8);
first_dots = max(max(lm_label));
disp(first_dots);

%calcules a quina peça correspon el nombre de punts

last_dots = dots - first_dots;
%disp(last_dots);

pieza = ['Aquesta peça es la ', num2str(first_dots),' ', num2str(last_dots)];
disp(pieza);






