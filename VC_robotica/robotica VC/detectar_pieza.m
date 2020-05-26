%Pregunta per terminal quina jugada de mostra vol que s'efectui, l'usuari
%ha d'escriure un nombre dels mostrats i es realitza aquella jugada
correcte = 0;
while(correcte == 0)
prompt = " Escull una de les jugades: (1) (2) (3) (4) (5) (6) (7)";
jugada = input(prompt);

im0 = imread("./jugadas/joc_0.jpeg");
if (jugada == 1)
   im1 = imread("./jugadas/joc_11.jpeg");
   im2 = imread("./jugadas/joc_12.jpeg");
   correcte = 1;
elseif (jugada == 2)
   im1 = imread("./jugadas/joc_21.jpeg");
   im2 = imread("./jugadas/joc_22.jpeg");
   correcte = 1;
elseif (jugada == 3)
   im1 = imread("./jugadas/joc_31.jpeg");
   im2 = imread("./jugadas/joc_32.jpeg");
   correcte = 1;
elseif (jugada == 4)
   im1 = imread("./jugadas/joc_41.jpeg");
   im2 = imread("./jugadas/joc_42.jpeg");
   correcte = 1;
elseif (jugada == 5)
   im1 = imread("./jugadas/joc_51.jpeg");
   im2 = imread("./jugadas/joc_52.jpeg");
   correcte = 1;
elseif (jugada == 6)
   im1 = imread("./jugadas/joc_61.jpeg");
   im2 = imread("./jugadas/joc_62.jpeg");
   correcte = 1;
elseif (jugada == 7)
   im1 = imread("./jugadas/joc_71.jpeg");
   im2 = imread("./jugadas/joc_72.jpeg");
   correcte = 1;
else
    error = ['El numero seleccionat no correspon amb cap jugada'];
    disp(error);
    correcte = 0;
end
end

procesa_imatges(im0,im1);
procesa_imatges(im1,im2);

function procesa_imatges(im1,im2)
    im1 = rgb2gray(im1);
    im2 = rgb2gray(im2);

    kernel = ones(5,5);

    figure,imshow(im1);
    figure,imshow(im2);

    %trobem quina ha estat la nova peca colocada
    im = im2 - im1;
    im = im < 90;
    im = imcomplement(im);
    im = imerode(im,kernel);

    %trobem les seves coordenades
    [yi,xi] = find(im,600,'first');
    [yf,xf] = find(im,600,'last');
    yi = min(yi);
    xi = min(xi);
    yf = max(yf);
    xf = max(xf);

    width = xf - xi;
    height = yf- yi;

    %figure(3),imshow(im);
   
    %retallem la peca de la imatge original 
    im = imcrop(im2,[xi yi width height]);

    %_______codi per llegir la peca______
    % treshold i tractament de la imatge
    im = im < 130;
    im = imerode(im,kernel);
    im = imdilate(im,kernel);

    %dividir la peca en dos per poder llegir els punts
    n = size(im,2);
    m = size(im,1);

    if (n > m)
        half_im1 = im(:,1:fix(n/2)-fix(0.20*n/2));
        half_im2 = im(:,fix(n/2)+fix(0.20*n/2):n);
    end

    if (n < m)
        half_im1 = im(1:fix(m/2)-fix(0.20*m/2),1:n-fix(0.2*n));
        half_im2 = im(fix(m/2)+fix(0.20*m/2):m,1:n-fix(0.2*n));
    end

    %figure(4),imshow(half_im1);
    %figure(5),imshow(half_im2);

    % contes punts de la primera mitad de la imatge
    lm_label = bwlabel(half_im1,8);
    dots1 = max(max(lm_label));
    %disp(dots1);

    % contes punts de la segona meitat de la imatge
    lm_label = bwlabel(half_im2,8);
    dots2 = max(max(lm_label));
    %disp(dots2);

    pieza = ['La nova peça es la ', num2str(dots1),' ', num2str(dots2)];
    disp(pieza);
    pause(2);
end







