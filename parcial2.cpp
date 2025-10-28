#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <tuple>
#include <cstdlib>
using namespace std;

// ===================== MODELOS =====================
struct Usuario {
    int idUsuario;
    string nombre;
    string correoElectronico;
    string contrasena;
    string direccion;
    string metodoDePago;
};

struct Producto {
    int idProducto;
    string nombre;
    string descripcion;
    double precio;
    int stock;
};

struct Comentario {
    int idComentario;
    int idProducto;     
    string usuario;     
    string contenido;
    string fecha;       
};

struct CarritoItem {
    int idProducto;
    int cantidad;
    double precioUnitario;
};

struct CarritoDeCompras {
    int idCarrito;
    int idUsuario;
    vector<CarritoItem> items;
    double subtotal;
    double impuestos;
};

// ===================== VECTORES =====================
vector<Usuario> USUARIOS;
vector<Producto> PRODUCTOS;
vector<Comentario> COMENTARIOS;
vector<CarritoDeCompras> CARRITOS;

// ===================== UTILIDADES =====================
string upperCopy(string s){
    for (char &c : s) {
        c = (char)toupper((unsigned char)c);
    }
    return s;
}
// Mayúsculas

tuple<int,int,int> ConvertirFecha(const string& ddmmyyyy){
    int d=0,m=0,y=0; 
    char c1='/',c2='/'; 
    stringstream ss(ddmmyyyy); 
    ss>>d>>c1>>m>>c2>>y; 
    return {y,m,d};
}// Convertir fecha para comparar

bool compararFecha(const string& f, const string& desde){ 
    return ConvertirFecha(f) >= ConvertirFecha(desde); 
}// Comparar fechas

Usuario* buscarUsuario(const string& correo, const string& clave) {
    for (auto &u : USUARIOS) {
        if (u.correoElectronico == correo && u.contrasena == clave) {
            return &u;
        }
    }
    return nullptr;
}

Producto* buscarProducto(int id) {
    for (auto &p : PRODUCTOS) {
        if (p.idProducto == id) {
            return &p;
        }
    }
    return nullptr;
}

int idPorNombreProducto(const string& nombre) {
    for (const auto &p : PRODUCTOS) {
        if (p.nombre == nombre) {
            return p.idProducto;
        }
    }
    return -1;
}


CarritoDeCompras* carritoDeUsuario(int idUsuario) {
    for (auto &c : CARRITOS) {
        if (c.idUsuario == idUsuario) {
            return &c;
        }
    }
    return nullptr;
}
int siguienteIdCarrito() {
    int mx = 0;
    for (auto &c : CARRITOS) {
        mx = max(mx, c.idCarrito);
    }
    return mx + 1;
}


// ===================== INICIALIZACIÓN  =====================
void initUsuarios(){
    USUARIOS = {
        {1,"Juan Pérez","juan.perez@email.com","Qwerty123","Carrera 45 #10-20","Tarjeta de crédito"},
        {2,"Ana Gómez","ana.gomez@email.com","Pass456","Calle 21 #35-50","PayPal"},
        {3,"Carlos Ruiz","carlos.ruiz@email.com","Segura789","Avenida Principal #100","Transferencia bancaria"},
        {4,"Sofía Martínez","sofia.martinez@email.com","Clave987","Calle 8 #20-30","Efectivo"},
        {5,"Diego Fernández","diego.fernandez@email.com","Contra654","Carrera 77 #40-60","Tarjeta débito"}
    };
}

void initProductos(){
    PRODUCTOS = {
        {  1,"Laptop","Portátil con pantalla Full HD y SSD de 512GB",89999,10},
        {  2,"Smartphone","Teléfono con cámara de 108MP y carga rápida",49950,20},
        {  3,"Tablet","Dispositivo con pantalla táctil de 10 pulgadas",29999,15},
        {  4,"Auriculares","Audífonos inalámbricos con cancelación de ruido",12999,25},
        {  5,"Teclado","Teclado mecánico con iluminación RGB",8999,30},
        {  6,"Mouse","Ratón inalámbrico con sensor óptico de alta precisión",5999,50},
        {  7,"Monitor","Pantalla LED 4K de 27 pulgadas",49900,12},
        {  8,"Impresora","Láser multifuncional con Wi-Fi",17999,18},
        {  9,"Cámara","Cámara digital con lente profesional",79999,8},
        { 10,"Smartwatch","Reloj inteligente con GPS y monitoreo cardíaco",19999,22},
        { 11,"Silla Gamer","Silla ergonómica ajustable con soporte lumbar",29999,14},
        { 12,"Microondas","Horno microondas con múltiples funciones",12999,40},
        { 13,"Refrigerador","Frigorífico doble puerta con sistema No Frost",119999,5},
        { 14,"Lavadora","Lavadora automática con capacidad de 10 kg",59999,7},
        { 15,"Cafetera","Hace café delicioso; fácil de usar",14999,20},
        { 16,"Drone","Drone con cámara; muy divertido",79999,6},
        { 17,"Bocina","Altavoz portátil con buena conexión",9999,25},
        { 18,"Televisor","Televisor LED UHD Smart TV",219999,9},
        { 19,"Consola","Consola de videojuegos con mando inalámbrico",169999,13},
        { 20,"Router","Router Wi-Fi de doble banda con antenas externas",7999,28},
        { 21,"Fuente Solar","Panel solar portátil con batería integrada",24999,4},
        { 22,"Control Remoto","Mando universal para TV y dispositivos",2499,50},
        { 23,"Termostato","Termostato digital programable",9999,22},
        { 24,"Smart Lock","Cerradura electrónica con huella digital",19999,6},
        { 25,"Proyector","Proyector LED con resolución Full HD",29999,12},
        { 26,"Switch Ethernet","Switch de red de 8 puertos",5999,38},
        { 27,"Reloj Digital","Reloj inteligente con pantalla AMOLED",8999,26},
        { 28,"Luces LED","Tiras LED RGB con control remoto",3999,55},
        { 29,"Estabilizador","Estabilizador de voltaje para dispositivos electrónicos",15999,10},
        { 30,"Cargador Inalámbrico","Base de carga inalámbrica rápida",4999,30},
        { 31,"HDD Externo","Disco duro portátil de 4TB",17999,15},
        { 32,"Micrófono","Micrófono profesional para grabación",14999,7},
        { 33,"Altavoz Inteligente","Asistente de voz con altavoz integrado",12999,20},
        { 34,"Antena Wi-Fi","Amplificador de señal inalámbrico",7999,33},
        { 35,"Climatizador","Aire acondicionado portátil con control remoto",29999,5},
        { 36,"Raspberry Pi","Kit de desarrollo con Raspberry Pi 4",12999,19},
        { 37,"Capturadora","Placa de captura de video en alta resolución",19999,8},
        { 38,"Smart Plug","Enchufe inteligente compatible con asistentes virtuales",3999,42},
        { 39,"Timbre Inteligente","Timbre con cámara y conexión a Wi-Fi",14999,10},
        { 40,"Cámara de Seguridad","Cámara IP con visión nocturna",17999,16},
        { 41,"Cargador Inalámbrico","Base de carga inalámbrica rápida",4999,30},
        { 42,"HDD Externo","Disco duro portátil de 4TB",17999,15},
        { 43,"Micrófono","Micrófono profesional para grabación",14999,7},
        { 44,"Altavoz Inteligente","Asistente de voz con altavoz integrado",12999,20},
        { 45,"Antena Wi-Fi","Amplificador de señal inalámbrico",7999,33},
        { 46,"Climatizador","Aire acondicionado portátil con control remoto",29999,5},
        { 47,"Raspberry Pi","Kit de desarrollo con Raspberry Pi 4",12999,19},
        { 48,"Capturadora","Placa de captura de video en alta resolución",19999,8},
        { 49,"Smart Plug","Enchufe inteligente compatible con asistentes virtuales",3999,42},
        { 50,"Timbre Inteligente","Timbre con cámara y conexión a Wi-Fi",14999,10}
    };
}

void initComentarios(){
    COMENTARIOS.clear();
    auto A = [&](int idC,const string& prod,const string& usuario,const string& texto,const string& fecha){
        COMENTARIOS.push_back({idC, idPorNombreProducto(prod), usuario, texto, fecha});
    };
    A( 1,"Laptop","Juan Pérez","Excelente rendimiento; muy rápida. ¡Me encanta!","01/05/2025");
    A( 2,"Smartphone","Ana Gómez","Buena cámara pero la batería dura poco.","03/05/2025");
    A( 3,"Tablet","Carlos Ruiz","No me gustó; pantalla de baja calidad.","05/05/2025");
    A( 4,"Auriculares","Sofía Martínez","Sonido aceptable pero el material parece frágil.","06/05/2025");
    A( 5,"Teclado","Diego Fernández","Muy buen teclado mecánico; excelente respuesta.","08/05/2025");
    A( 6,"Mouse","Ana Gómez","El sensor no es tan preciso como esperaba.","10/05/2025");
    A( 7,"Monitor","Carlos Ruiz","Colores vibrantes y buena resolución. Muy satisfecho.","12/05/2025");
    A( 8,"Impresora","Juan Pérez","Tarda mucho en imprimir; no me convence.","13/05/2025");
    A( 9,"Cámara","Sofía Martínez","Increíble calidad de imagen; fotos súper nítidas.","15/05/2025");
    A(10,"Smartwatch","Diego Fernández","Buena batería; pero la pantalla no es muy brillante.","18/05/2025");
    A(11,"Silla Gamer","Lucía Rodríguez","Comodidad espectacular; perfecto para largas sesiones de juego.","20/05/2025");
    A(12,"Microondas","Andrés Ramírez","Calienta bien pero hace mucho ruido.","22/05/2025");
    A(13,"Refrigerador","María García","Espacioso y enfría rápido; muy recomendado.","24/05/2025");
    A(14,"Lavadora","Javier Martínez","Lava bien pero el ciclo es muy largo.","26/05/2025");
    A(15,"Cafetera","Carolina López","Hace café delicioso; fácil de usar.","28/05/2025");
    A(16,"Drone","Daniel Castro","Muy divertido pero la batería dura poco.","30/05/2025");
    A(17,"Bocina","Paola Herrera","Sonido potente y buena conexión.","01/06/2025");
    A(18,"Cámara","Esteban Rojas","Perfecta para grabaciones profesionales.","03/06/2025");
    A(19,"Televisor","Fernanda Sánchez","Imagen excelente pero el sonido podría mejorar.","05/06/2025");
    A(20,"Power Bank","Camilo Torres","Carga bien pero es un poco pesada.","07/06/2025");
    A(21,"HDD Externo","Gabriela Suárez","Gran capacidad de almacenamiento; funciona rápido.","09/06/2025");
    A(22,"Memoria USB","Raúl Espinosa","Buen tamaño pero la velocidad de transferencia es baja.","11/06/2025");
    A(23,"Router","Verónica Mendoza","Señal potente; cubre toda la casa.","13/06/2025");
    A(24,"Control Remoto","Fabio Jiménez","Cómodo y resistente; ideal para gaming.","15/06/2025");
    A(25,"Fuente Solar","Ricardo Vargas","Funciona bien pero los cables son muy cortos.","17/06/2025");
    A(26,"SSD Interno","Silvia Gómez","Velocidad increíble; mi PC va mucho más rápido ahora.","19/06/2025");
    A(27,"Parlantes 2.1","Martín Aguilar","Sonido envolvente; muy buena compra.","21/06/2025");
    A(28,"Cámara Web","Valentina Pérez","Imagen clara pero el micrófono es deficiente.","23/06/2025");
    A(29,"Procesador","José Ramírez","Rendimiento impecable; ideal para gaming y diseño.","25/06/2025");
    A(30,"Router","Natalia Correa","Buenas prestaciones pero la instalación fue complicada.","27/06/2025");
    A(31,"Memoria RAM","Julio Fernández","Expande muy bien el rendimiento del sistema.","29/06/2025");
    A(32,"Fuente Solar","Amanda Castro","Energía confiable pero la batería es pequeña.","01/07/2025");
    A(33,"Control Remoto","Pedro Duarte","Fácil de usar; reconoce muchos dispositivos.","03/07/2025");
    A(34,"Termostato","Isabela Medina","Regula bien la temperatura; intuitivo de usar.","05/07/2025");
    A(35,"Smart Lock","Oscar Rodríguez","Seguridad y tecnología en un solo dispositivo.","07/07/2025");
    A(36,"Proyector","Cristina Vargas","Imagen nítida pero requiere una sala oscura.","09/07/2025");
    A(37,"Switch Ethernet","Mario Hernández","Buena velocidad de conexión; estable.","11/07/2025");
    A(38,"Reloj Digital","Sofía Ramírez","Pantalla atractiva pero la batería dura poco.","13/07/2025");
    A(39,"Luces LED","Andrea Gutiérrez","Buenas opciones de colores; buen diseño.","15/07/2025");
    A(40,"Estabilizador","Pablo Medina","Protege bien contra variaciones de voltaje.","17/07/2025");
    A(41,"Cargador Inalámbrico","Patricia López","Carga rápido pero requiere posicionamiento preciso.","19/07/2025");
    A(42,"HDD Externo","Gonzalo Espinoza","Mucho espacio; resistente y confiable.","21/07/2025");
    A(43,"Micrófono","Elena Herrera","Calidad de sonido profesional; ideal para podcast.","23/07/2025");
    A(44,"Altavoz Inteligente","Diego Soto","Responde bien a comandos de voz; útil en casa.","25/07/2025");
    A(45,"Antena Wi-Fi","Miguel Rojas","Amplifica bien la señal pero el rango es limitado.","27/07/2025");
    A(46,"Climatizador","Estefanía Carrillo","Enfría rápido pero es algo ruidoso.","29/07/2025");
    A(47,"Raspberry Pi","Manuel Vargas","Perfecto para proyectos electrónicos y programación.","31/07/2025");
    A(48,"Capturadora","Luisa Mejía","Ideal para streaming y grabaciones en alta calidad.","02/08/2025");
    A(49,"Smart Plug","Victoria Torres","Muy práctico; fácil de conectar y configurar.","04/08/2025");
    A(50,"Timbre Inteligente","Federico Montoya","Buena cámara; útil para seguridad.","06/08/2025");
}

// ===================== FUNCIONES=====================
void listarProductos(int umbral){
    cout << "\n--- Productos con stock < " << umbral << " ---\n";
    for (const auto &p : PRODUCTOS)
        if (p.stock < umbral)
            cout << "["<<p.idProducto<<"] "<<p.nombre<<" | stock="<<p.stock
                 <<" | $"<<fixed<<setprecision(2)<<p.precio<<"\n";
    system("pause");
    system("cls");
}

void buscarComentarios(const string& desde){
    cout << "\n--- Comentarios desde " << desde << " ---\n";
    for (const auto &c : COMENTARIOS){
        if (!compararFecha(c.fecha, desde)) continue;
        const Producto* p = buscarProducto(c.idProducto);
        cout << "#" << c.idComentario << " [" << c.fecha << "] "
             << c.usuario << " sobre "
             << (p? p->nombre : "Producto?")
             << ": " << c.contenido << "\n";
    }
    system("pause");
    system("cls");
} 

void listarUsuarios(){
    cout << "\n--- Usuarios (nombre en MAYÚSCULAS) ---\n";
    for (const auto &u : USUARIOS)
        cout << "("<<u.idUsuario<<") "<< upperCopy(u.nombre)
             <<" | correo="<<u.correoElectronico
             <<" | direccion="<<u.direccion
             <<" | metodoDePago="<<u.metodoDePago<<"\n";
    system("pause");
    system("cls");
}
//Como en el ejercicio no se especifica como se calcula el total, se asume que es subtotal + impuestos, y los impuestos se toma el 19% por el iva.
void agregarAlCarro(Usuario& u, int idProducto, int cantidad){
    Producto* prod = buscarProducto(idProducto);
    if (!prod){ cout << "Producto no existe.\n"; return; }
    if (cantidad<=0){ cout << "Cantidad inválida.\n"; return; }
    if (prod->stock < cantidad){ cout << "Stock insuficiente. Disponible: "<<prod->stock<<"\n"; return; }

    CarritoDeCompras* car = carritoDeUsuario(u.idUsuario);
    if (!car){
        CARRITOS.push_back({siguienteIdCarrito(), u.idUsuario, {}, 0.0, 0.0});
        car = &CARRITOS.back();
    }

    bool found=false;
    for (auto &it: car->items) if (it.idProducto==idProducto){ it.cantidad+=cantidad; found=true; break; }
    if (!found) car->items.push_back({idProducto, cantidad, prod->precio});

    prod->stock -= cantidad;

    double sub=0.0; for (auto &it: car->items) sub += it.precioUnitario * it.cantidad;
    car->subtotal = sub;
    car->impuestos = sub * 0.19;

    cout << "Agregado. Carrito #" << car->idCarrito
         << " | Subtotal: $" << fixed << setprecision(2) << car->subtotal
         << " | Impuestos: $" << car->impuestos << "\n";
    system("pause");
    system("cls");
}

void verCarrito(Usuario& u){
    CarritoDeCompras* car = carritoDeUsuario(u.idUsuario);
    if (!car || car->items.empty()){ cout << "\nCarrito vacío.\n"; return; }
    cout << "\n--- Carrito de " << u.nombre << " (ID " << car->idCarrito << ") ---\n";
    for (auto &it: car->items){
        auto* p = buscarProducto(it.idProducto);
        cout << "["<<it.idProducto<<"] " << (p? p->nombre : "Producto?")
             << " x" << it.cantidad
             << " @ $" << fixed << setprecision(2) << it.precioUnitario
             << " = $" << (it.precioUnitario * it.cantidad) << "\n";
    }
    cout << "Subtotal: $" << car->subtotal << " | Impuestos: $" << car->impuestos << "\n";
    system("pause");
    system("cls");
}

// ===================== MENÚ =====================
void menuUsuario(Usuario& u){
    while (true){
        cout << "\n=== Menú ===\n";
        cout << "1) Listar productos con bajo stock (<15)\n";
        cout << "2) Comentarios desde una fecha \n";
        cout << "3) Listar usuarios \n";
        cout << "4) Agregar producto al carrito \n";
        cout << "5) Listar productos del carrito\n";
        cout << "6) Salir\n";
        cout << "Opción: ";
        int op; if(!(cin>>op)) return;

        if (op==1){
            system("cls");
            listarProductos(15);
        } else if (op==2){
            system("cls");
            cout << "Fecha (dd/mm/aaaa): "; 
            string f; 
            cin>>f; 
            buscarComentarios(f);
        } else if (op==3){
            system("cls");
            listarUsuarios();
        } else if (op==4){
            system("cls");
            int idP,cant; 
            cout<<"ID producto: ";
            cin>>idP; 
            cout<<"Cantidad: ";
            cin>>cant; 
            agregarAlCarro(u,idP,cant);
        } else if (op==5){
            system("cls");
            verCarrito(u);
        } else if (op==6){
            cout << "Hasta luego.\n"; break;
        } else {
            cout << "Opción inválida.\n";
        }
    }
}

// ===================== MAIN =====================
int main(){
    
    initUsuarios();
    initProductos();
    initComentarios();
    while (true) {
    string correo, clave;
    cout << "Correo: ";
    cin >> correo;
    cout << "Clave: ";
    cin >> clave;

    Usuario* u = buscarUsuario(correo, clave);
    if (u) {
        cout << "Inicio de sesión exitoso. Bienvenido, " << u->nombre << "!\n";
        system("pause");
        system("cls");
        menuUsuario(*u);
        break; 
    } else {
        system("cls");
        cout << "Usuario o contraseña inválidos. Intente nuevamente.\n";
        system("pause");
        system("cls");
    }
    }
return 0;
}
