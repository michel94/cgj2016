Aspectos Importantes da implementação:
Como mostrei na aula, implementei uma classe para animação sequencial.
São implementadas:
	- Uma classe abstracta Animation, com as seguintes funções virtuais puras: update (que recebe a quantidade de tempo que passou e aplica as transformações respectivas ao objecto) e reverse (que devolve a animação inversa dessa instância).
	- Uma classe abstracta AnimController, que pode registar-se como callback da classe Animation, sendo chamada a sua função onFinish quando a respectiva animação termina a sua execução.
As translações e as rotações são representadas pelas classes Translation e Rotation, respectivamente. Estas são subclasses de Animation e implementam os métodos necessários. Quando terminam, chamam a função onFinish do respectivo controlador que lhes tiver sido atribuído.
Uma sequência de animações é também uma animação. Para poder executar animações em série, a classe AnimSequence extende, simultaneamente, Animation e AnimController. Desta maneira, pode ser registada como uma animação normal mas controlar as suas sub-animações internas, avançando para a animação seguinte quando uma acaba.
Existe ainda a classe AnimManager, que extende a classe AnimController, e que é usada para obter uma instância singleton, utilizada de forma global para invocação de animações.


A classe Mesh que é pedida tem o nome Model (pois já tinha sido criada anteriormente).

