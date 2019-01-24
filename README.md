# Despliegue-3D
Proyecto 2 - ICG
Brian Torres - CI 24042974

==========================

Al ejecutar la aplicaci�n, encontrar� un men� que contiene las siguientes funcionalidades:
*    Abrir modelo: Al presionar el bot�n 'Abrir modelo' o presionar las teclas "l" se despliega una ventana (filechooser de Windows) para cargar un archivo .off u .obj a ser le�do.
*    Seleccionar modelo: Puede seleccionar el �ndice del modelo cargado para trabajar con �l posteriormente. Cabe destacar que este �ndice comienza en 0 y al iniciar la aplicaci�n tiene el valor de -1, indicando que no hay ning�n modelo cargado.

    El 'Modo selecci�n' puede ser accedido desde el men� presionando el bot�n 'Seleccionar' o presionando la tecla s. En cualquier momento se puede regresar al men� para seleccionar otro modelo presionando el bot�n 'Volver al Men�' o presionando la tecla m. En el 'Modo selecci�n' se encuentran las siguientes funcionalidades:
*    Cambiar proyecci�n: Desde el sub-men� 'Proyecci�n' se debe seleccionar alguna opci�n de la lista desplegable (Perspectiva u ortogonal) y seguidamente seleccionar el bot�n 'Activar' para cambiar la proyecci�n de despliegue de los modelos. 
*    Z-Buffer: Si se desea activar o desactivar el Z-Buffer de todos los modelos, se debe seleccionar el bot�n 'Activar' desde su respectivo sub-men�. En el mismo se mostrar� si la opci�n est� activada (con un check) o no (con un gui�n).
*    Back-Face Culling: Si se desea activar o desactivar el Back-Face Culling de todos los modelos, se debe seleccionar el bot�n 'Activar' desde su respectivo sub-men�. En el mismo se mostrar� si la opci�n est� activada (con un check) o no (con un gui�n).
*    Cambiar estilo de despliegue: Se debe seleccionar alguna opci�n de la lista desplegable (Puntos, mallado o tri�ngulos rellenos) para cambiar el estilo del modelo seleccionado.
*    Cambiar sombreado: Se debe seleccionar alguna opci�n de la lista desplegable (Gouraud, Flat) para cambiar el sombreado hacia los modelos.
*    Bounding Box: Si se desea mostrar u ocultar el bounding box de un modelo seleccionado, se debe presionar el bot�n 'Activar' desde su respectivo sub-men�. Se puede cambiar el color del mismo desde sus paletas.
*    Normales: Si se desean mostrar u ocultar las normales de un modelo seleccionado, se debe presionar el bot�n 'Activar' desde su respectivo sub-men�. Se puede cambiar el color de las mismas desde sus paletas.
*    Escalamiento: Se debe especificar un factor de escalamiento en el campo 'Escalar'. El valor por defecto de este factor es 1.00, el cual indica el tama�o original del modelo al ser cargado. Un factor menor a 1 har� que el modelo se haga m�s peque�o; y un factor mayor a 1 har� que el modelo se haga m�s grande. Cabe destacar que un modelo es escalado en los 3 ejes por igual. Como atajo, se puede aumentar el tama�o de un modelo haciendo scroll hacia arriba con el mouse; y disminuir el tama�o del mismo haciendo scroll hacia abajo.
*    Trasladar: Se puede modificar la posici�n del modelo seleccionado ajustando el eje (x, y o z) deseado. Inicialmente los valores de estos ejes son (0.0,0.0,0.0), indicando que el modelo cargado se encuentra en el centro de la pantalla.
*    Rotaci�n: Se puede rotar libremente un modelo seleccionado moviendo la esfera de rotaci�n como se desee. Si se desea auto-rotar un modelo, se debe presionar el bot�n 'Auto-rotaci�n'. Esta �ltima se realiza en base al eje Y.
*    Cambiar color: Se puede cambiar el color ambiental, difuso y especular del modelo seleccionado desde las paletas como se desee.
*    Luz: Se puede modificar la posici�n de la luz ajustando el eje (x, y o z) deseado y apagar o encender las mismas. 
*    Cambiar color:  Se puede cambiar el color ambiental, difuso y especular de la luz desde las paletas como se desee.

===========================
Puede seleccionar el bot�n 'Salir' o presionar la tecla Esc en cualquier momento para cerrar la aplicaci�n.
