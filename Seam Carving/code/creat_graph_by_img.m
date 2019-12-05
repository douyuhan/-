function f=creat_graph_by_img(im_gray)
    lin=size(im_gray,1);
    col=size(im_gray,2);
    
    
    E=E1(im_gray);
    S(1:lin)=1; %les cotes a partir du noeud S vers la premiere colonne d'image, 
				%donc les noeuds "source" tous sont S, son numéro est 1
    T(1:lin)=num_node_in_graph(im_gray,1,1):num_node_in_graph(im_gray,lin,1);
				%les noeuds "destination" de ces cotes, ici ce sont la premiere colonne d'image
    Weight=inf*ones([1,lin]);  %les couts (énergie) correspondants aux cotes ci-dessus
	
    %  ajouter la premiere colonne d'image dans le graph
    S=[S,num_node_in_graph(im_gray,1,1):num_node_in_graph(im_gray,lin,1)];
    T=[T,num_node_in_graph(im_gray,1,2):num_node_in_graph(im_gray,lin,2)];
    Weight=[Weight,E(:,1)'];
	
	%  les colonnes generales
    for j=2:(col-1)

        %%vers droite 
		%l'operation sur toute les lignnes a une fois (a l'aide de l'extension de matrice)
        S=[S,num_node_in_graph(im_gray,1,j):num_node_in_graph(im_gray,lin,j)]; 
        T=[T,num_node_in_graph(im_gray,1,j+1):num_node_in_graph(im_gray,lin,j+1)];
        Weight=[Weight,E(1:lin,j)'];

        %%vers gouche
        S=[S,num_node_in_graph(im_gray,1,j):num_node_in_graph(im_gray,lin,j)];
        T=[T,num_node_in_graph(im_gray,1,j-1):num_node_in_graph(im_gray,lin,j-1)];
        Weight=[Weight,inf*ones([1,lin])];

        %%vers gouche-bas
        S=[S,num_node_in_graph(im_gray,1,j):num_node_in_graph(im_gray,lin-1,j)];
        T=[T,num_node_in_graph(im_gray,2,j-1):num_node_in_graph(im_gray,lin,j-1)];
        Weight=[Weight,inf*ones([1,lin-1])];

        %%vers gouche-haut
        S=[S,num_node_in_graph(im_gray,2,j):num_node_in_graph(im_gray,lin,j)];
        T=[T,num_node_in_graph(im_gray,1,j-1):num_node_in_graph(im_gray,lin-1,j-1)];
        Weight=[Weight,inf*ones([1,lin-1])];
    end

    %% la derniere colonne d'image
    j=col;

	%%vers gouche
	S=[S,num_node_in_graph(im_gray,1,j):num_node_in_graph(im_gray,lin,j)];
	T=[T,num_node_in_graph(im_gray,1,j-1):num_node_in_graph(im_gray,lin,j-1)];
	Weight=[Weight,inf*ones([1,lin])];

	%%vers gouche-bas
	S=[S,num_node_in_graph(im_gray,1,j):num_node_in_graph(im_gray,lin-1,j)];
	T=[T,num_node_in_graph(im_gray,2,j-1):num_node_in_graph(im_gray,lin,j-1)];
	Weight=[Weight,inf*ones([1,lin-1])];

	%%vers gouche-haut
	S=[S,num_node_in_graph(im_gray,2,j):num_node_in_graph(im_gray,lin,j)];
	T=[T,num_node_in_graph(im_gray,1,j-1):num_node_in_graph(im_gray,lin-1,j-1)];
	Weight=[Weight,inf*ones([1,lin-1])];

	%%vers T (vers droite)
	S=[S,num_node_in_graph(im_gray,1,j):num_node_in_graph(im_gray,lin,j)];
	T=[T,2*ones([1,lin])]; %Les "destination" tous sont le noeud T (son numero est 2)
	Weight=[Weight,inf*ones([1,lin])];


    f=digraph(S,T,Weight); %graph final
  
end