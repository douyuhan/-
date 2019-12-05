clear
close all
clc

N_para=2.5;
N=614;
col=288;
lin=384;
frame_buffer=zeros([col,lin,N]);
objet_fond=zeros([col,lin,N]);
O=zeros([col,lin]);

%%"VMAX" et "VMIN" sont les parametres pour eviter du debordement (dans le cas de sur-exposition)
VMAX=255*ones([col,lin]); %VMAX=255 parce que il y a 8 niveaux du gris
VMIN=2*ones([col,lin]);   %VMIN=2 (valeur typique)


for i=1:N
    if(i<10)
        im=imread(['image_','00',mat2str(i),'.jpg']);
        frame_buffer(:,:,i)=rgb2gray(im);
    elseif(i<100)
        im=imread(['image_','0',mat2str(i),'.jpg']);
        frame_buffer(:,:,i)=rgb2gray(im);
    else
        im=imread(['image_',mat2str(i),'.jpg']);
        frame_buffer(:,:,i)=rgb2gray(im);
    end
end

moyenne=mean(frame_buffer,3);
v=std(frame_buffer,0,3);

for i=1:N
    moyenne=moyenne-(moyenne>frame_buffer(:,:,i))+(moyenne<frame_buffer(:,:,i)); %mise a jour du estimateur M
    O(:,:,i)=abs(frame_buffer(:,:,i)-moyenne);
	v=v+(v<(N_para*O(:,:,i)))-(v>(N_para*O(:,:,i))); %mise a jour du estimateur V ("N_para" est un param que on choisit)
    v=max(min(v,VMAX),VMIN);                         %correction l'effet de sur-exposition (VMAX=255, VMIN=2)
	objet_fond(:,:,i)=255*(O(:,:,i)<v);              %decision de pixel
end

for i=1:N
    imshow(objet_fond(:,:,i),[])
    pause(0.033);
end
