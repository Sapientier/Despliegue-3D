# Despliegue-3D
Proyecto 2 - ICG
Brian Torres - CI 24042974

==========================

Al ejecutar la aplicación, encontrará un menú que contiene las siguientes funcionalidades:
*    Abrir modelo: Al presionar el botón 'Abrir modelo' o presionar las teclas "l" se despliega una ventana (filechooser de Windows) para cargar un archivo .off u .obj a ser leído.
*    Seleccionar modelo: Puede seleccionar el índice del modelo cargado para trabajar con él posteriormente. Cabe destacar que este índice comienza en 0 y al iniciar la aplicación tiene el valor de -1, indicando que no hay ningún modelo cargado.

    El 'Modo selección' puede ser accedido desde el menú presionando el botón 'Seleccionar' o presionando la tecla s. En cualquier momento se puede regresar al menú para seleccionar otro modelo presionando el botón 'Volver al Menú' o presionando la tecla m. En el 'Modo selección' se encuentran las siguientes funcionalidades:
*    Cambiar proyección: Desde el sub-menú 'Proyección' se debe seleccionar alguna opción de la lista desplegable (Perspectiva u ortogonal) y seguidamente seleccionar el botón 'Activar' para cambiar la proyección de despliegue de los modelos. 
*    Z-Buffer: Si se desea activar o desactivar el Z-Buffer de todos los modelos, se debe seleccionar el botón 'Activar' desde su respectivo sub-menú. En el mismo se mostrará si la opción está activada (con un check) o no (con un guión).
*    Back-Face Culling: Si se desea activar o desactivar el Back-Face Culling de todos los modelos, se debe seleccionar el botón 'Activar' desde su respectivo sub-menú. En el mismo se mostrará si la opción está activada (con un check) o no (con un guión).
*    Cambiar estilo de despliegue: Se debe seleccionar alguna opción de la lista desplegable (Puntos, mallado o triángulos rellenos) para cambiar el estilo del modelo seleccionado.
*    Cambiar sombreado: Se debe seleccionar alguna opción de la lista desplegable (Gouraud, Flat) para cambiar el sombreado hacia los modelos.
*    Bounding Box: Si se desea mostrar u ocultar el bounding box de un modelo seleccionado, se debe presionar el botón 'Activar' desde su respectivo sub-menú. Se puede cambiar el color del mismo desde sus paletas.
*    Normales: Si se desean mostrar u ocultar las normales de un modelo seleccionado, se debe presionar el botón 'Activar' desde su respectivo sub-menú. Se puede cambiar el color de las mismas desde sus paletas.
*    Escalamiento: Se debe especificar un factor de escalamiento en el campo 'Escalar'. El valor por defecto de este factor es 1.00, el cual indica el tamaño original del modelo al ser cargado. Un factor menor a 1 hará que el modelo se haga más pequeño; y un factor mayor a 1 hará que el modelo se haga más grande. Cabe destacar que un modelo es escalado en los 3 ejes por igual. Como atajo, se puede aumentar el tamaño de un modelo haciendo scroll hacia arriba con el mouse; y disminuir el tamaño del mismo haciendo scroll hacia abajo.
*    Trasladar: Se puede modificar la posición del modelo seleccionado ajustando el eje (x, y o z) deseado. Inicialmente los valores de estos ejes son (0.0,0.0,0.0), indicando que el modelo cargado se encuentra en el centro de la pantalla.
*    Rotación: Se puede rotar libremente un modelo seleccionado moviendo la esfera de rotación como se desee. Si se desea auto-rotar un modelo, se debe presionar el botón 'Auto-rotación'. Esta última se realiza en base al eje Y.
*    Cambiar color: Se puede cambiar el color ambiental, difuso y especular del modelo seleccionado desde las paletas como se desee.
*    Luz: Se puede modificar la posición de la luz ajustando el eje (x, y o z) deseado y apagar o encender las mismas. 
*    Cambiar color:  Se puede cambiar el color ambiental, difuso y especular de la luz desde las paletas como se desee.

===========================
Puede seleccionar el botón 'Salir' o presionar la tecla Esc en cualquier momento para cerrar la aplicación.
