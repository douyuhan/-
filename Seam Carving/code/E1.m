function f = E1(img)
    lin=size(img,1);
    col=size(img,2);
	
    img_X=[img(:,2:col),zeros([lin,1])]; %deplacement d'image a gauche pour calculer le gradient horizontal,
										 %et remplissage la colonne la plus droite à 0
    X=img_X-img;  %gradient horizontal
	
    img_Y=[img(2:lin,:);zeros([1,col])]; %deplacement d'image vers haut pour calculer le gradient vertical,
										 %et remplissage la colonne la plus basse à 0
    Y=img_Y-img;  %gradient vertical
	
    f=abs(X)+abs(Y); %gradient total, qui est E1 dans l'article
    f=double(f);
end
