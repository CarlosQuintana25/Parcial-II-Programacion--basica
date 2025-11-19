#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <tuple>
#include <cstdlib>
#include <fstream>
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

string convertirFechaaDDMMYYYY(const string& fechas) {
    return fechas.substr(8, 2) + "/" + fechas.substr(5, 2) + "/" + fechas.substr(0, 4);
}// Cambiar formato de fecha de yyyy/mm/dd a dd/mm/yyyy


// ===================== INICIALIZACIÓN  =====================
void initUsuarios() {
    USUARIOS.clear();
    ifstream file("Usuarios.txt");
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir Usuarios.txt\n";
        return;
    }

    string linea;
    getline(file, linea); // saltar encabezado

    while (getline(file, linea)) {
        if (linea.empty()) continue;

        stringstream ss(linea);
        string id, nombre, correo, contrasena, direccion, metodoPago;

        getline(ss, id, ',');
        getline(ss, nombre, ',');
        getline(ss, correo, ',');
        getline(ss, contrasena, ',');
        getline(ss, direccion, ',');
        getline(ss, metodoPago, ',');

        USUARIOS.push_back({
            stoi(id),
            nombre,
            correo,
            contrasena,
            direccion,
            metodoPago
        });
    }

    file.close();
}

void initProductos() {
    PRODUCTOS.clear();
    ifstream file("Productos.txt");
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir Productos.txt\n";
        return;
    }

    string linea;
    getline(file, linea); // saltar encabezado

    while (getline(file, linea)) {
        if (linea.empty()) continue;

        stringstream ss(linea);
        string id, nombre, desc, precio, stock;

        getline(ss, id, ',');
        getline(ss, nombre, ',');
        getline(ss, desc, ',');
        getline(ss, precio, ',');
        getline(ss, stock, ',');

        PRODUCTOS.push_back({
            stoi(id),
            nombre,
            desc,
            stod(precio),
            stoi(stock)
        });
    }

    file.close();
}

void initComentarios() {
    COMENTARIOS.clear();
    ifstream file("Comentarios.txt");
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir Comentarios.txt\n";
        return;
    }

    string linea;
    getline(file, linea); // saltar encabezado

    while (getline(file, linea)) {
        if (linea.empty()) continue;

        stringstream ss(linea);
        string id, producto, usuario, texto, fecha;

        getline(ss, id, ',');
        getline(ss, producto, ',');
        getline(ss, usuario, ',');
        getline(ss, texto, ',');
        getline(ss, fecha, ',');

        int idProd = idPorNombreProducto(producto);

        COMENTARIOS.push_back({
            stoi(id),
            idProd,
            usuario,
            texto,
            convertirFechaaDDMMYYYY(fecha)
        });
    }

    file.close();
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
