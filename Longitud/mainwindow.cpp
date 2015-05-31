#include "io.h" //creat, open, write, read
#include "fcntl.h" //O_TEXT, O_BINARY
#include <stdio.h> //cout,cin
#include "string.h" //strlen
#include "sys\stat.h" //S_IREAD, S_IWRITE
#include "stdlib.h" //exit
#include <cstdlib>
#include <iostream>
//const char delim_C='*';
//const char delim_R='@';
typedef int BOOL ;
# define TRUE 1;
# define FALSE 0;

struct Proveedor
{
 char codigo[5];
 char nombre[25];
 char apellido[20];
 char telefono[9];
 char ciudad[20];
}pv;


BOOL escribirArchivo(char nomArch[])
{
 int fd, nReg=0;
 char res='s';

 if((fd=creat(nomArch,S_IWRITE))<0)
 {
     return FALSE;
 }

 while(res=='s')
 {
     system("cls");

     cout << "\n"<<"Registro: "<<++nReg;

     cout<<"\n\t    Codigo:";
     cin.getline(pv.codigo,5);
     cin.ignore();

     cout<<"\n\t    Nombre:";
     cin.getline(pv.nombre,25);
     cin.ignore();

     cout<<"\n\t    Apellido:";
     cin.getline(pv.apellido,20);
     cin.ignore();

     cout<<"\n\t    Telefono:";
     cin.getline(pv.telefono,9);
     cin.ignore();

     cout<<"\n\t    Ciudad:";
     cin.getline(pv.ciudad,20);
     cin.ignore();

     write(fd,&pv,sizeof(struct Proveedor));

     cout<<"\n\n Desea seguir registrando (s/n): ";
     cin>>res;
     cin.ignore();
 }

 close(fd);

 system("cls");

 return TRUE;
}

void leerArchivo(char nomArch[])
{
 int fd, nReg=0;

 if((fd=open(nomArch,S_IREAD))<0)
 {
     cout<<"Error al abrir el Archivo";
 }

 while(read(fd,&pv,sizeof(struct Proveedor))>0)
 {
     cout<<endl<<"Registro "<<++nReg;

     cout<<"\n\t Codigo: ";
     cout<<pv.codigo;

     cout<<"\n\t Nombre: ";
     cout<<pv.nombre;

     cout<<"\n\t Apellido: ";
     cout<<pv.apellido;

     cout<<"\n\t Telefono: ";
     cout<<pv.telefono;

     cout<<"\n\t Ciudad: ";
     cout<<pv.ciudad;
 }
 close(fd);

}

BOOL buscarRegistro(char nomArch[], char dato[])
{
 int fd, nReg=0, band=FALSE;

 if((fd=open(nomArch,S_IREAD))<0)
 {
     cout<<"Error al abrir el Archivo";
 }

 while(read(fd,&pv,sizeof(struct Proveedor))>0)
 {
     if( stricmp(pv.codigo,dato)==0 || stricmp(pv.nombre,dato)==0 || stricmp(pv.apellido,dato)==0 || stricmp(pv.telefono,dato)==0 || stricmp(pv.ciudad,dato)==0 )
     {
         cout<<endl<<"Registro "<<++nReg;

         cout<<"\n\t Codigo: ";
         cout<<pv.codigo;

         cout<<"\n\t Nombre: ";
         cout<<pv.nombre;

         cout<<"\n\t Apellido: ";
         cout<<pv.apellido;

         cout<<"\n\t Telefono: ";
         cout<<pv.telefono;

         cout<<"\n\t Ciudad: ";
         cout<<pv.ciudad;

         band=TRUE;
     }
     else
         nReg++;
 }
 close(fd);

 if(band)
     return TRUE;

 return FALSE;
}

int main(int argc, char* argv[])
{
 char nomArch[20], dato[20];


 system("color 2f");


 cout<<"\n Ingrese nombre del archivo: ";
 cin.getline(nomArch,20);
 cin.ignore();


 if(escribirArchivo(nomArch))
     cout<<"\n Se registro con exito";
 else
     cout<<"\n Error en abrir archivo";

 cout<<endl<<endl;
 leerArchivo(nomArch);

 cout<<endl<<endl;
 cout<<"Ingrese dato a buscar: ";
 cin.getline(dato,20);
 cin.ignore();

 if(buscarRegistro(nomArch,dato))
     cout<<"\n El dato se encontro en el Archivo";
 else
     cout<<"\n El dato no se encontro en el Archivo";

 cout<<endl<<endl;

 system("PAUSE");
 return 0;

}
