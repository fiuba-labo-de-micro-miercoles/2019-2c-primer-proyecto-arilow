# Repositorio para la documentación de proyectos de 
# 6609/8607 Laboratorio de Microcontroladores

## Integrantes
- Echegaray, Ignacio 	#98868
- Lowy, Ariel Dario		#98298 
- Rack, Lucas Alexis 	#99425

## Proyecto: cuadrúpedo inalámbrico
El presente proyecto tiene como objetivo diseñar e implementar un robot cuadrúpedo de ocho ejes capaz de desplazarse para adelante, para atrás y rotar en ambos sentidos. Se trabajará el movimiento de los ejes de cada extremidad por separado para luego integrarlas en el movimiento completo deseado. Cada eje poseerá un servomotor que puede ser posicionado según la instrucción en un ángulo entre 0 y 180 grados a la velocidad deseada.

El dispositivo será controlado con un mando inalámbrico mediante un módulo bluetooth.

En cuanto al sensor, siempre estará posicionado en el sentido del desplazamiento del conjunto, con el fin de evitar colisiones. En caso de desplazarse en reversa, no será contemplado su funcionamiento. De detectar una posible colisión, provocará una parada de todos los motores y analizará una posible ruta sin obstáculos.

![imagen](https://i1.wp.com/makezine.com/wp-content/uploads/2017/05/QuadFigureB.png?resize=620%2C190&ssl=1)