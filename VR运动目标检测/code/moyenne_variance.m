clear
close all
clc

N=614;  %nb de frames
col=288;%nb de colonnes
lin=384;%nb de lignes

frame_buffer=zeros([col,lin,N]); %initialisation
objet_fond=zeros([col,lin,N]);
moyenne=zeros([col,lin]);
ecart_type=zeros([col,lin]);
lambda=1;  %pour calculer le seuil

for i=1:N  %lecture de tous les frames par le nom d'images
    if(i<10)
        im=imread(['image_','00',mat2str(i),'.jpg']); 
        frame_buffer(:,:,i)=rgb2gray(im); %mettre RGB en niveau du gris
    elseif(i<100)
        im=imread(['image_','0',mat2str(i),'.jpg']);
        frame_buffer(:,:,i)=rgb2gray(im);
    else
        im=imread(['image_',mat2str(i),'.jpg']);
        frame_buffer(:,:,i)=rgb2gray(im);
    end
end

moyenne=mean(frame_buffer,3);     %calcul du moyenne de tous les pixels de tous les frames
ecart_type=std(frame_buffer,0,3); %calcul de la variance de tous les pixels de tous les frames

for i=1:N
    diff=abs(frame_buffer(:,:,i)-moyenne); 
    objet_fond(:,:,i)=255*(diff<lambda*ecart_type); %comparaison au seuil
	%si la difference entre sa valeur et la moyenne est inferiere que le seuil, on le considere comme le fond
end

for i=1:N
    imshow(objet_fond(:,:,i),[])
    pause(0.033);
end