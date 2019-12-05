function num = num_node_in_graph(img,i,j)
        lin=size(img,1);  %nb de ligne d'image
		
        num=(j-1)*lin+i+2;%l'index du pixel img(i,j) dans le vecteur S ("source" pour graph)
						  %Ici, on fixe le noeud S et T à l'index 1 et 2 respectivement,
						  %donc l'index de pixel d'image commence a 3
end

