#include <iostream>
#include <fstream>
#include <string>

class Guardable {
public:
    virtual void guardarEnArchivo() = 0;
};

class Autor : public Guardable {
private:
    std::string dni;
    std::string nombre;

public:
    Autor(const std::string& d, const std::string& n) : dni(d), nombre(n) {}

    const std::string& getDNI() const { return dni; }
    const std::string& getNombre() const { return nombre; }

    void guardarEnArchivo() override {
        std::ofstream archivo("autores.txt", std::ios::app);
        if (archivo.is_open()) {
            archivo << dni << "," << nombre << "\n";
            archivo.close();
        }
        else {
            std::cerr << "No se pudo abrir el archivo para guardar el autor.\n";
        }
    }
};

class Usuario : public Guardable {
private:
    std::string nombre;
    std::string contrasena;

public:
    Usuario(const std::string& n, const std::string& c) : nombre(n), contrasena(c) {}

    const std::string& getNombre() const { return nombre; }
    const std::string& getContrasena() const { return contrasena; }

    void guardarEnArchivo() override {
        std::ofstream archivo("usuarios.txt", std::ios::app);
        if (archivo.is_open()) {
            archivo << nombre << "," << contrasena << "\n";
            archivo.close();
        }
        else {
            std::cerr << "No se pudo abrir el archivo para guardar el usuario.\n";
        }
    }
};

class Noticia : public Guardable {
private:
    std::string titulo;
    std::string detalle;
    std::string autorNombre;

public:
    Noticia(const std::string& t, const std::string& d, const std::string& aNombre)
        : titulo(t), detalle(d), autorNombre(aNombre) {}

    const std::string& getTitulo() const { return titulo; }
    const std::string& getDetalle() const { return detalle; }
    const std::string& getAutorNombre() const { return autorNombre; }

    void guardarEnArchivo() override {
        std::ofstream archivo("noticias.txt", std::ios::app);
        if (archivo.is_open()) {
            archivo << titulo << "," << detalle << "," << autorNombre << "\n";
            archivo.close();
        }
        else {
            std::cerr << "No se pudo abrir el archivo para guardar la noticia.\n";
        }
    }
};

class Comentario : public Guardable {
private:
    std::string texto;
    std::string usuarioNombre;
    std::string tituloNoticia;

public:
    Comentario(const std::string& txt, const std::string& uNombre, const std::string& tNoticia)
        : texto(txt), usuarioNombre(uNombre), tituloNoticia(tNoticia) {}

    const std::string& getTexto() const { return texto; }
    const std::string& getUsuarioNombre() const { return usuarioNombre; }
    const std::string& getTituloNoticia() const { return tituloNoticia; }

    void guardarEnArchivo() override {
        std::ofstream archivo("comentarios.txt", std::ios::app);
        if (archivo.is_open()) {
            archivo << tituloNoticia << "," << texto << "," << usuarioNombre << "\n";
            archivo.close();
        }
        else {
            std::cerr << "No se pudo abrir el archivo para guardar el comentario.\n";
        }
    }
};

void listarNoticias() {
    std::ifstream archivo("noticias.txt");
    if (archivo.is_open()) {
        std::string linea;
        while (std::getline(archivo, linea)) {
            std::cout << "Noticia: " << linea << std::endl;

            std::string tituloNoticia = linea.substr(0, linea.find(','));

            std::ifstream archivoComentarios("comentarios.txt");
            if (archivoComentarios.is_open()) {
                std::string lineaComentario;
                while (std::getline(archivoComentarios, lineaComentario)) {
                    if (lineaComentario.find(tituloNoticia) != std::string::npos) {
                        std::cout << "    Comentario: " << lineaComentario << std::endl;
                    }
                }
                archivoComentarios.close();
            }
            else {
                std::cerr << "No se pudo abrir el archivo comentarios.txt" << std::endl;
            }
        }
        archivo.close();
    }
    else {
        std::cerr << "No se pudo abrir el archivo noticias.txt" << std::endl;
    }
}

void listarUsuarios() {
    std::ifstream archivo("usuarios.txt");
    if (archivo.is_open()) {
        std::string linea;
        while (std::getline(archivo, linea)) {
            std::cout << "Usuario: " << linea << std::endl;
        }
        archivo.close();
    }
    else {
        std::cerr << "No se pudo abrir el archivo usuarios.txt" << std::endl;
    }
}

void borrarRegistro(const std::string& archivo, const std::string& identificador) {
    std::ifstream archivoOriginal(archivo);
    std::ofstream archivoTemp("temp.txt");

    if (archivoOriginal.is_open() && archivoTemp.is_open()) {
        std::string linea;
        while (std::getline(archivoOriginal, linea)) {
            if (linea.find(identificador) == std::string::npos) {
                archivoTemp << linea << "\n";
            }
        }
        archivoOriginal.close();
        archivoTemp.close();
        remove(archivo.c_str());
        rename("temp.txt", archivo.c_str());
    }
    else {
        std::cerr << "No se pudo abrir el archivo para borrar el registro.\n";
    }
}

void registrarDatos() {
    int tipo;
    std::cout << "Ingrese 1 para registrar un usuario, 2 para registrar un autor: ";
    std::cin >> tipo;

    if (tipo == 1) {
        std::string nombre, contrasena;
        std::cout << "Ingrese nombre del usuario: ";
        std::cin.ignore();
        std::getline(std::cin, nombre);
        std::cout << "Ingrese contraseña del usuario: ";
        std::getline(std::cin, contrasena);

        Usuario usuario(nombre, contrasena);
        usuario.guardarEnArchivo();
    }
    else if (tipo == 2) {
        std::string dni, nombre;
        std::cout << "Ingrese DNI del autor: ";
        std::cin.ignore();
        std::getline(std::cin, dni);
        std::cout << "Ingrese nombre del autor: ";
        std::getline(std::cin, nombre);

        Autor autor(dni, nombre);
        autor.guardarEnArchivo();

        std::string titulo, detalle;
        std::cout << "Ingrese titulo de la noticia: ";
        std::getline(std::cin, titulo);
        std::cout << "Ingrese detalle de la noticia: ";
        std::getline(std::cin, detalle);

        Noticia noticia(titulo, detalle, nombre);
        noticia.guardarEnArchivo();
    }
    else {
        std::cerr << "Opcion no valida.\n";
    }
}

void comentarNoticia() {
    std::string nombreUsuario, contrasena;
    std::cout << "Ingrese su nombre de usuario: ";
    std::cin.ignore();
    std::getline(std::cin, nombreUsuario);
    std::cout << "Ingrese su contraseña: ";
    std::getline(std::cin, contrasena);

    std::ifstream archivoUsuarios("usuarios.txt");
    bool usuarioValido = false;
    if (archivoUsuarios.is_open()) {
        std::string linea;
        while (std::getline(archivoUsuarios, linea)) {
            if (linea.find(nombreUsuario) != std::string::npos && linea.find(contrasena) != std::string::npos) {
                usuarioValido = true;
                break;
            }
        }
        archivoUsuarios.close();
    }

    if (!usuarioValido) {
        std::cerr << "Usuario o contraseña incorrectos.\n";
        return;
    }

    std::string tituloNoticia, textoComentario;
    std::cout << "Ingrese el titulo de la noticia a comentar: ";
    std::getline(std::cin, tituloNoticia);
    std::cout << "Ingrese su comentario: ";
    std::getline(std::cin, textoComentario);

    Comentario comentario(textoComentario, nombreUsuario, tituloNoticia);
    comentario.guardarEnArchivo();
}

void mostrarMenu() {
    std::cout << "\nSeleccione una opcion:\n";
    std::cout << "1. Registrar datos\n";
    std::cout << "2. Listar noticias\n";
    std::cout << "3. Listar usuarios\n";
    std::cout << "4. Comentar noticia\n";
    std::cout << "5. Borrar noticia\n";
    std::cout << "6. Borrar usuario\n";
    std::cout << "0. Salir\n";
    std::cout << "Opcion: ";
}

int main() {
    int opcion;
    std::string identificador;
    do {
        mostrarMenu();
        std::cin >> opcion;

        switch (opcion) {
        case 1:
            registrarDatos();
            break;
        case 2:
            listarNoticias();
            break;
        case 3:
            listarUsuarios();
            break;
        case 4:
            comentarNoticia();
            break;
        case 5:
            std::cout << "Ingrese el titulo de la noticia a borrar: ";
            std::cin.ignore();
            std::getline(std::cin, identificador);
            borrarRegistro("noticias.txt", identificador);
            break;
        case 6:
            std::cout << "Ingrese el nombre del usuario a borrar: ";
            std::cin.ignore();
            std::getline(std::cin, identificador);
            borrarRegistro("usuarios.txt", identificador);
            break;
        case 0:
            std::cout << "Saliendo...\n";
            break;
        default:
            std::cout << "Opcion no valida\n";
            break;
        }
    } while (opcion != 0);

    return 0;
}
