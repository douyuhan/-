clear
close all
clc

N=600;
lin=288;
col=384;
Eijk=zeros([lin,col,N]);
lambda=100*ones([lin,col]);  %parametre a choisir

for i=1:N
    if(i<10)
        im=imread(['image_','00',mat2str(i),'.jpg']);
        Eijk(:,:,i)=imgaussfilt(rgb2gray(im));
    elseif(i<100)
        im=imread(['image_','0',mat2str(i),'.jpg']);
        Eijk(:,:,i)=imgaussfilt(rgb2gray(im));
    else
        im=imread(['image_',mat2str(i),'.jpg']);
        Eijk(:,:,i)=imgaussfilt(rgb2gray(im));
    end
end

EiJk=[Eijk(:,2:col,:),Eijk(:,col,:)]; %deplacement du cube (3D) pour la utilisation suivante de E(i,j+1,k)
EIJk=[EiJk(2:lin,:,:);EiJk(lin,:,:)]; %E(i+1,j+1,k)
EIjk=[Eijk(2:lin,:,:);Eijk(lin,:,:)]; %E(i+1,j,k)
EijK=cat(3,Eijk(:,:,2:N),Eijk(:,:,N));%E(i,j,k+1)
EiJK=[EijK(:,2:col,:),EijK(:,col,:)]; %E(i,j+1,k+1)
EIJK=[EiJK(2:lin,:,:);EiJK(lin,:,:)]; %E(i+1,j+1,k+1)
EIjK=[EijK(2:lin,:,:);EijK(lin,:,:)]; %E(i+1,j,k+1)

Ex=0.25*(EiJk-Eijk+EIJk-EIjk+EiJK-EijK+EIJK-EIjK); %approximation de Ex
Ey=0.25*(EIjk-Eijk+EIJk-EiJk+EIjK-EijK+EIJK-EiJK); %approximation de Ey
Ek=0.25*(EijK-Eijk+EIjK-EIjk+EiJK-EiJk+EIJK-EIJk); %approximation de Ek

% for i=1:N
%     subplot(3,1,1)
%     imshow(Ex(:,:,i),[])
%     subplot(3,1,2)
%     imshow(Ey(:,:,i),[])
%     subplot(3,1,3)
%     imshow(Ek(:,:,i),[])
%     pause(0.033);
% end

for frame=1:N
    Uk=zeros([lin,col]); %initialisation
    Vk=zeros([lin,col]);
    for i=1:1
        Ui_j=[Uk(1,:);Uk(1:lin-1,:)]; %deplacement de matrice pour la utilisation suivante de U(i-1,j,k)
        UiJ=[Uk(:,2:col),Uk(:,col)];       %U(i,j+1,k)
        UIj=[Uk(2:lin,:);Uk(lin,:)];  	   %U(i+1,j,k)
        Uij_=[Uk(:,1),Uk(:,1:col-1)]; 	   %U(i,j-1,k)
        Ui_j_=[Uij_(1,:);Uij_(1:lin-1,:)]; %U(i-1,j-1,k)
        Ui_J=[UiJ(1,:);UiJ(1:lin-1,:)];	   %U(i-1,j+1,k)
        UIJ=[UiJ(2:lin,:);UiJ(lin,:)];     %U(i+1,j+1,k)
        UIj_=[Uij_(2:lin,:);Uij_(lin,:)];  %U(i+1,j+1,k)
        
        Um=(1/6)*(Ui_j+UiJ+UIj+Uij_)+(1/12)*(Ui_j_+Ui_J+UIJ+UIj_); %calcul de la moyenne de "U" locale
        
        Vi_j=[Vk(1,:);Vk(1:lin-1,:)]; %meme operation sur V
        ViJ=[Vk(:,2:col),Vk(:,col)];
        VIj=[Vk(2:lin,:);Vk(lin,:)];
        Vij_=[Vk(:,1),Vk(:,1:col-1)];
        Vi_j_=[Vij_(1,:);Vij_(1:lin-1,:)];
        Vi_J=[ViJ(1,:);ViJ(1:lin-1,:)];
        VIJ=[ViJ(2:lin,:);ViJ(lin,:)];
        VIj_=[Vij_(2:lin,:);Vij_(lin,:)];
        
        Vm=(1/6)*(Vi_j+ViJ+VIj+Vij_)+(1/12)*(Vi_j_+Vi_J+VIJ+VIj_); %calcul de la moyenne de "U" locale
        
        Uk=Uk-Ex(:,:,frame).*((Ex(:,:,frame).*Um+Ey(:,:,frame).*Vm+Ek(:,:,frame))./(Ex(:,:,frame).^2+Ey(:,:,frame).^2+lambda));
        Vk=Vk-Ey(:,:,frame).*((Ex(:,:,frame).*Um+Ey(:,:,frame).*Vm+Ek(:,:,frame))./(Ex(:,:,frame).^2+Ey(:,:,frame).^2+lambda));
    end
   
    img=flowToColor(Uk,Vk);
    imshow(img,[]);
    pause(0.033)
end

