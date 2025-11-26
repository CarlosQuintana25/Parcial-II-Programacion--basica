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
#include <locale>

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



// ===================== VECTORES =====================
vector<Usuario> USUARIOS;
vector<Producto> PRODUCTOS;
vector<Comentario> COMENTARIOS;


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
    getline(file, linea); 

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
    getline(file, linea); 

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
    getline(file, linea); 

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
void agregarAlCarro(Usuario& u, int idProducto, int cantidad){
    Producto* prod = buscarProducto(idProducto);
    if (!prod){ cout << "Producto no existe.\n"; return; }
    if (cantidad <= 0){ cout << "Cantidad inválida.\n"; return; }
    if (prod->stock < cantidad){
        cout << "Stock insuficiente. Disponible: " << prod->stock << "\n";
        return;
    }
    double total = prod->precio * cantidad;

    ofstream file("Carrito.txt", ios::app);
    if (!file.is_open()) {
        cout << "ERROR: No se pudo abrir Carrito.txt\n";
        return;
    }

    file << u.idUsuario << ","
         << prod->idProducto << ","
         << prod->nombre << ","
         << cantidad << ","
         << prod->precio << ","
         << total << "\n";

    file.close();


    prod->stock -= cantidad;
    cout << "Producto agregado al carrito y guardado en Carrito.txt\n";
    system("pause");
    system("cls");
}


void verCarrito(Usuario& u){
    ifstream file("Carrito.txt");
    if (!file.is_open()) {
        cout << "No hay carrito o no se pudo abrir Carrito.txt\n";
        return;
    }

    cout << "\n=== Carrito del usuario " << u.nombre << " ===\n";

    string linea;
    bool encontrado = false;

    while (getline(file, linea)) {
        if (linea.empty()) continue;

        stringstream ss(linea);
        string idU, idProd, nombreProd, cant, precio, total;

        getline(ss, idU, ',');
        getline(ss, idProd, ',');
        getline(ss, nombreProd, ',');
        getline(ss, cant, ',');
        getline(ss, precio, ',');
        getline(ss, total, ',');

        if (stoi(idU) == u.idUsuario) {
            encontrado = true;
            cout << "Producto: " << nombreProd
                 << " | Cantidad: " << cant
                 << " | Precio: " << precio
                 << " | Total: " << total
                 << "\n";
        }
    }

    if (!encontrado)
        cout << "Carrito vacío.\n";

    file.close();
    system("pause");
    system("cls");
}
// ===================== REPORTES (PARCIAL 3) =====================
 
void reporteProductosStockBajo() {
    cout << "\n--- 5 productos con el stock más bajo ---\n";
    vector<Producto> listaOrdenada = PRODUCTOS;

    sort(listaOrdenada.begin(), listaOrdenada.end(),
        [](const Producto& uno, const Producto& dos) {
            return uno.stock < dos.stock;
        }
    );

    int cantidadAMostrar = (listaOrdenada.size() < 5 ? listaOrdenada.size() : 5);

    for (int i = 0; i < cantidadAMostrar; i++) {
        cout << listaOrdenada[i].idProducto 
             << " | " << listaOrdenada[i].nombre
             << " | Stock: " << listaOrdenada[i].stock << "\n";
    }

    system("pause");
    system("cls");
}


void reporteCantidadComentariosFecha() {
    cout << "\nIngrese la fecha (dd/mm/aaaa): ";
    string fecha;
    cin >> fecha;

    int contador = 0;
    for (const auto& c : COMENTARIOS) {
        if (c.fecha == fecha) contador++;
    }

    cout << "\n--- Cantidad de comentarios ---\n";
    cout << "Fecha: " << fecha << "\n";
    cout << "Cantidad de comentarios: " << contador << "\n";

    system("pause");
    system("cls");
}


void reportePrecioMaxMin() {
    if (PRODUCTOS.empty()) {
        cout << "No hay productos cargados.\n";
        return;
    }

    double minPrecio = PRODUCTOS[0].precio;
    double maxPrecio = PRODUCTOS[0].precio;

    for (const auto& p : PRODUCTOS) {
        minPrecio = min(minPrecio, p.precio);
        maxPrecio = max(maxPrecio, p.precio);
    }

    cout << "\n--- Precios Máximo y Mínimo ---\n";
    cout << "Precio mínimo: " << fixed << setprecision(2) << minPrecio << "\n";
    cout << "Precio máximo: " << fixed << setprecision(2) << maxPrecio << "\n";

    system("pause");
    system("cls");
}

void menuReportes() {
    while (true) {
        cout << "\n=== Reportes ===\n";
        cout << "1) Mostrar los 5 productos con menor stock\n";
        cout << "2) Cantidad de comentarios en una fecha\n";
        cout << "3) Precio máximo y mínimo de los productos\n";
        cout << "4) Volver\n";
        cout << "Opción: ";

        int op;
        cin >> op;

        if (op == 1) {
            system("cls");
            reporteProductosStockBajo();
        }
        else if (op == 2) {
            system("cls");
            reporteCantidadComentariosFecha();
        }
        else if (op == 3) {
            system("cls");
            reportePrecioMaxMin();
        }
        else if (op == 4) {
            system("cls");
            return;
        }
        else {
            cout << "Opción inválida.\n";
        }
    }
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
        cout << "6) Reportes\n";
        cout << "7) Salir\n";
        cout << "Opción: ";
        int op; 
        if(!(cin>>op)) return;

        if (op==1){
            system("cls");
            listarProductos(15);
        } 
        else if (op==2){
            system("cls");
            cout << "Fecha (dd/mm/aaaa): ";
            string f;
            cin >> f;
            buscarComentarios(f);
        } 
        else if (op==3){
            system("cls");
            listarUsuarios();
        } 
        else if (op==4){
            system("cls");
            int idP, cant;
            cout << "ID producto: ";
            cin >> idP;
            cout << "Cantidad: ";
            cin >> cant;
            agregarAlCarro(u, idP, cant);
        } 
        else if (op==5){
            system("cls");
            verCarrito(u);
        } 
        else if (op==6){
            system("cls");
            menuReportes();
        }
        else if (op==7){
            cout << "Hasta luego.\n";
            break;
        }
        else {
            cout << "Opción inválida.\n";
        }
    }
}



// ===================== MAIN =====================
int main(){
    setlocale(LC_ALL, "");
    system("chcp 65001 > nul"); 
    
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
