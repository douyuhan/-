function cut=draw_cut(img,S_f,pixel_cut) 
%S_f est l'ensemble des pixels associes au noeud S
%pixel_cut: nb de pixel dans chaque ligne a supprimer lors de chaque iteration (也即chemin的宽度)
%cut: matrice de meme dimension d'image. 
%Les premieres "pixel_cut" colonnes contiennent les pixels associes au noeud T
										 
    cut=zeros([size(img,1),size(img,2)]); %meme dimension d'image
	
	%initialision
    for i=1:size(S_f,1)
        if (S_f(i)>2)         %l'index de pixel d'image commence a 3 
            cut(S_f(i)-2)=255;%au debut, on met tous les pixel d'image associes au noeud S a blanc(255),
							  %donc les pixels d'image associe au noeud T reste a noir(0)
        end
    end
	
    test=sum(cut,1); %le vecteur dont l'element est la somme de chaque colonne de "cut"
					 %行向量，按"cut"的第一个维度(行)求和
    p=find(test<255*size(img,1));%现在cut的元素只有2种值:0或255。若一列的和=255*行数,则表示这一列的pixel全属于S
						%这里是为了找到一列中存在属于T的pixel的列,
						%chercher tous les colonnes ou il y a au moins un pixel associe au noeud T
						%（Ces pixels associes au T doit etre supprimer）


%%下面的if...else...end是为了将切割路径上的像素置为255，其余的都置为0
%注意：S和T的位置并不是任意的，在切割线左边的点都属于S，右边的都属于T，
%因此可以通过 水平移位再相减 提取出路径，而其他位置的像素值都为0。
%由于位置的规律性，下面进行移位之后进行减法的时候不会出现的负数结果

	%从左向右，找到第一个含有属于T的列的位置（即p(1)），将从该列开始切割,	
	%默认切割该位置左边的像素，若p(1)>pixel_cut，则左边的位置足够切割
	%On suppose que les colonnes a gauche de cette position vont etre supprimer. 
	
	if (p(1)>pixel_cut) %p(1) est l'index du premier element de "p", 
						%c'est l'index de la premiere colonne qui contient les pixels associé au noeud "T"
						%On va commence le cut a cette colonne. 
						
        cut_p=[cut(:,(1+pixel_cut):size(img,2)),zeros([size(img,1),pixel_cut])]; 
		%deplacer le "cut" original les "pixel_cut" colonnes a gauche, et remplir les dernieres colonnes avec 0
        cut=cut-cut_p; %apres la soustraction, seulement les pixels dans le chemin ont la valeur 255, 
		               %autre pixels sont a 0
	
	%Si p(1)<=pixel_cut，il n'y aura pas assez de colonnes a gquche pour les supprimer,
	%donc, on va deplacer le "cut" original a droite	
    else  
        cut_p=[255*ones([size(img,1),pixel_cut]),cut(:,1:size(img,2)-pixel_cut)];
		%deplacer le "cut" original les "pixel_cut" colonnes a droite, et remplir les premieres colonnes avec 0
        cut=cut_p-cut;
    end
	
	%只有切割路径上的像素值为255，其他地方都为0

end