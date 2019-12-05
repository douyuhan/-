clear
close all
clc

cut_step=1; %pas d'iteration dans "draw_cut"

im=imread('banc.jpg');
im_display=im;
im_gray=rgb2gray(im);
figure
imshow(im_gray);

for iteratif=1:50
    E=E1(im_gray);
    G=creat_graph_by_img(im_gray);
    [m_f,v_f,S_f,T_f]=maxflow(G,1,2,'searchtrees');
    cut=draw_cut(im_gray,S_f,cut_step);
    im1=im_display;   
    im1(:,:,1)=im1(:,:,1)+uint8(cut);  %pour illustrer le chemin (rouge)
    imshow(im1);
	
	%pour chaque ligne, on cherche les pixels associes au noeud "T" (a supprimer)
    for i=1:size(im_gray,1) 
        cut_lin=find(cut(i,:)==255); 
        im2(i,:,:)=[im(i,1:(cut_lin(1)-1),:),im(i,(cut_lin(cut_step)+1):size(im_gray,2),:)];%suppression
    end
	%après la boucle interne, cut_lin est le chemin à supprimer
	
    im_display=im2;
    clear im2;   %mettre les matrices ou le vecteur a 0 chaque fois apres le "cut", 
	clear im1;   %parce que le nombre de colonne diminue apres chaque iteration, il faut le mettre a jour
	clear cut_lin; 
	figure
    imshow(im_display);
    im_gray=rgb2gray(im_display);
    im=im_display;
end