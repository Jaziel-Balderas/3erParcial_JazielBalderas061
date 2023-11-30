#include <Windows.h>
#include "resource.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

struct nodo_eventos {
	char fecha[20];
	char evento[20];
	char numboletos[10];
	char precio[10];
	nodo_eventos* sig;
	nodo_eventos* ant;
};

nodo_eventos* lista_eventos = NULL, * auxiliar = NULL, * primero = NULL, * ultimo = NULL;

HINSTANCE hInst;

void agregar(nodo_eventos* auxiliar);

LRESULT CALLBACK VINICIOSESION(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VMENUOPCIONES(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VEVENTOSPROX(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VCOMPRABOLETO(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK VHISTORIALDECOMPRAS(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmdLine, int cShow) {
	HWND hIniciarSes = CreateDialog(hInst, MAKEINTRESOURCE(VENTANASESION), NULL, VINICIOSESION);
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	ShowWindow(hIniciarSes, cShow);
	while (GetMessage(&msg,hIniciarSes,NULL,NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

//VENTANA INICIO SESION
LRESULT CALLBACK VINICIOSESION(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_CLOSE: {
			int respuesta = MessageBox(hwnd, "¿Deseas cerrar el programa?", "Confirmacion", MB_YESNO | MB_ICONWARNING);
			if (respuesta == IDYES)
			{
				DestroyWindow(hwnd);
			}
		}break;

		case WM_DESTROY: {
			PostQuitMessage(0);
		}break;

		case WM_COMMAND: {
			if (LOWORD(wParam) == BTN_INICIO && HIWORD(wParam) == BN_CLICKED) {
				HWND hUsuario = GetDlgItem(hwnd, USUARIO);
				char Usu[20];
				GetWindowText(hUsuario, Usu,sizeof(Usu));
				int UsuLong = GetWindowTextLength(hUsuario);

				HWND hContra = GetDlgItem(hwnd, CONTRASENA);
				char Contra[20];
				GetWindowText(hContra, Contra, sizeof(Contra));
				int ContraLong = GetWindowTextLength(hContra);

				//VALIDAR QUE EL USUARIO Y LA CONTRASEÑA TENGAN MINIMO 4 CARACTERES
				if (UsuLong < 4) { 
					MessageBox(hwnd, "EL USUARIO DEBE TENER 4 CARACTERES MINIMO", "ERROR", MB_OK | MB_ICONERROR);
					break;
				}
				if (ContraLong < 4) {
					MessageBox(hwnd, "LA CONTRASEÑA DEBE TENER 4 CARACTERES MINIMO", "ERROR", MB_OK | MB_ICONERROR);
					break;
				}
				if (UsuLong >= 4 && ContraLong >= 4) {
					EndDialog(hwnd, 0);
					DialogBox(hInst, MAKEINTRESOURCE(MENU), hwnd, VMENUOPCIONES); //CIERRA EL INICIO DE SESION Y VA AL MENU
				}
			}
		}break;
	}
	return FALSE;
}

//VENTANA MENU
LRESULT CALLBACK VMENUOPCIONES(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_COMMAND: {
			if (LOWORD(wParam) == EVENTOS || LOWORD(wParam) == BN_CLICKED) {
				EndDialog(hwnd, 0);
				DialogBox(hInst, MAKEINTRESOURCE(LISEVENTOS), hwnd, VEVENTOSPROX); //ABRE LA VENTANA DE EVENTOS DEL MES
			}
			if (LOWORD(wParam) == COMPRAS || LOWORD(wParam) == BN_CLICKED) {
				EndDialog(hwnd, 0);
				DialogBox(hInst, MAKEINTRESOURCE(VentanaDeCompra), hwnd, VCOMPRABOLETO); //ABRE LA VENTANA DE COMPRAR BOLETOS

			}
			if (LOWORD(wParam) == MISCOMPRAS || LOWORD(wParam) == BN_CLICKED) {
				EndDialog(hwnd, 0);
				DialogBox(hInst, MAKEINTRESOURCE(VentanaHistorialCom), hwnd, VHISTORIALDECOMPRAS); //ABRE LA VENTANA DEL HISTORIAL DE COMPRAS
			}
			if (LOWORD(wParam) == SALIR || LOWORD(wParam) == BN_CLICKED) {
				DestroyWindow(hwnd);
				PostQuitMessage(0); //SALE DEL PROGRAMA
			}
		}break;
	}
	return FALSE;
}

//VENTANA DE EVENTOS DEL MES
LRESULT CALLBACK VEVENTOSPROX(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_INITDIALOG: {

			//INSERTAMOS LOS EVENTOS DEL MES EN EL LISTBOX

			HWND hLBEventos = GetDlgItem(hwnd, CONCIERTOSLISTA);
			SendMessage(hLBEventos, LB_INSERTSTRING, NULL, (LPARAM)"Duki     05/12/2023");
			SendMessage(hLBEventos, LB_INSERTSTRING, NULL, (LPARAM)"");
			SendMessage(hLBEventos, LB_INSERTSTRING, NULL, (LPARAM)"Luis Miguel     08/12/2023");
			SendMessage(hLBEventos, LB_INSERTSTRING, NULL, (LPARAM)"");
			SendMessage(hLBEventos, LB_INSERTSTRING, NULL, (LPARAM)"BZRP     15/12/2023");
			SendMessage(hLBEventos, LB_INSERTSTRING, NULL, (LPARAM)"");
			SendMessage(hLBEventos, LB_INSERTSTRING, NULL, (LPARAM)"Ariana Grande     21/12/2023");
			SendMessage(hLBEventos, LB_INSERTSTRING, NULL, (LPARAM)"");
			SendMessage(hLBEventos, LB_INSERTSTRING, NULL, (LPARAM)"Eminem     28/12/2023");
		}break;
		case WM_COMMAND: {
			if (LOWORD(wParam) == VOLVERMENU || LOWORD(wParam) == BN_CLICKED) {
				EndDialog(hwnd, 0);
				DialogBox(hInst, MAKEINTRESOURCE(MENU), hwnd, VMENUOPCIONES); //REGRESA AL MENU 
			}
		}break;
	}
	return FALSE;
}

//VENTANA COMPRAS BOLETOS
LRESULT CALLBACK VCOMPRABOLETO(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_INITDIALOG: {

			//ESTOS CHARS LOS INSERTAREMOS EN LOS COMBOBOX PARA SELECCIONAR LA INFORMACION Y GUARDARLA
			char Duki[] = "Duki";
			char LuisMiguel[] = "Luis Miguel";
			char BZRP[] = "Bzrp";
			char ArianaGrande[] = "Ariana Grande";
			char Eminem[] = "Eminem";

			char NA[] = "1. No aplicar descuento";
			char dosXuno[] = "2. 2x1";
			char descuentoprecio[] = "3. Descuento del 10%";

			char PrecioUni[] = "1500"; //PRECIO FIJO DE LOS BOLETOS

			HWND hEvento = GetDlgItem(hwnd, EVENTOCB);
			SendMessage(hEvento, CB_INSERTSTRING, 0, (LPARAM)Duki);
			SendMessage(hEvento, CB_INSERTSTRING, 1, (LPARAM)LuisMiguel);
			SendMessage(hEvento, CB_INSERTSTRING, 2, (LPARAM)BZRP);
			SendMessage(hEvento, CB_INSERTSTRING, 3, (LPARAM)ArianaGrande);
			SendMessage(hEvento, CB_INSERTSTRING, 4, (LPARAM)Eminem);

			HWND hDescPrecio = GetDlgItem(hwnd, DESCUENTO);
			SendMessage(hDescPrecio, CB_ADDSTRING, NULL, (LPARAM)NA);
			SendMessage(hDescPrecio, CB_ADDSTRING, NULL, (LPARAM)dosXuno);
			SendMessage(hDescPrecio, CB_ADDSTRING, NULL, (LPARAM)descuentoprecio);

			HWND hPrecioUnitario = GetDlgItem(hwnd, PRECIOU);
			SetWindowText(hPrecioUnitario, PrecioUni);
		}break;
		case WM_COMMAND: {
			if (LOWORD(wParam) == CALCULARTOTAL || LOWORD(wParam) == BN_CLICKED) {  
				bool validardatos = true;
				bool validarboletos = true;
				int nBoletos = 0;

				HWND CantidadBoletos = GetDlgItem(hwnd, NUMBOLETOS);
				char Boletos[20];
				GetWindowText(CantidadBoletos, Boletos, sizeof(Boletos));
				nBoletos = atoi(Boletos);
				int NumBoletosLong = GetWindowTextLength(CantidadBoletos);
				
				//VALIDA QUE SE SELECCIONE LA CANTIDAD DE BOLETOS
				if (NumBoletosLong == 0) {
					MessageBox(hwnd, "SELECCIONE LA CANTIDAD DE BOLETOS", "ADVERTENCIA", MB_OK | MB_ICONWARNING);
					break;
				}
				for (int i = 0; i < NumBoletosLong; i++) {
					if (isalpha(Boletos[i])) //BUSCA LETRAS CARACTER POR CARACTER
					{
						validarboletos = false; //SI HAYA UNA LETRA EL BOOL SERA FALSO Y NO DEJARA GUARDAR LA INFORMACION
						validardatos = false;
					}
					if (isdigit(Boletos[i])) { //VALIDA QUE HAYA NUMEROS
						validarboletos = true;
						validardatos = true;
					}
				} //VALIDA QUE LA CANTIDAD DE BOLETOS NO TENGA LETRAS
				if (validarboletos == false) {
					MessageBox(hwnd, "SOLO SE ACEPTAN NUMEROS EN LA CANTIDAD DE BOLETOS", "ERROR", MB_OK | MB_ICONERROR);
					break;
				}

				HWND hPromocion = GetDlgItem(hwnd, DESCUENTO);
				char hDes[20];
				GetWindowText(hPromocion, hDes, sizeof(hDes));
				int DescuentoLong = SendMessage(hPromocion, CB_GETCURSEL, 0, 0);
				if (DescuentoLong == CB_ERR) //VALIDA QUE SE SELECCIONE UNA OPCION EN EL DESCUENTO
				{
					MessageBox(hwnd, "SELECCIONA SI QUIERES APLICAR UN DESCUENTO", "ADVERTENCIA", MB_OK | MB_ICONWARNING);
					validardatos = false;
					break;
				}

				if (validardatos == true) { //SI CUMPLE CON LOS REQUISITOS SE HARA EL CALCULO DEL PRECIO
					
					int opcion = 0, subT = 0, IVACalculado = 0, PrecioFinal = 0;

					if (DescuentoLong == 0) { //SI SE SELECCIONO NO APLICAR DESCUENTO EN EL CB REALIZARA ESTA OPERACION
						subT = nBoletos * 1500;
						IVACalculado = subT * 0.16;
						PrecioFinal = IVACalculado + subT;
						SetDlgItemInt(hwnd, SUBTOTAL, subT, TRUE);
						SetDlgItemInt(hwnd, IVA, IVACalculado, TRUE);
						SetDlgItemInt(hwnd, TOTAL, PrecioFinal, TRUE);
					}
					if (DescuentoLong == 1) {//SI SE SELECCIONO 2X1 EN EL CB REALIZARA ESTA OPERACION
						if (nBoletos == 1) { //SI SOLO SE COMPRARA 1 BOLETO LANZARA ESTA ADVERTENCIA EN EL 2X1
							MessageBox(hwnd, "EL 2X1 SOLO APLICA PARA LA COMPRA DE 2 BOLETOS O MAS", "ERROR", MB_OK | MB_ICONERROR);
							break;
						}
						if ((nBoletos % 2) == 0) { //CORROBORAR SI LA CANTIDAD DE BOLETOS ES PAR
							subT = nBoletos * 750;
							IVACalculado = subT * 0.16;
							PrecioFinal = IVACalculado + subT;
							SetDlgItemInt(hwnd, SUBTOTAL, subT, FALSE);
							SetDlgItemInt(hwnd, IVA, IVACalculado, FALSE);
							SetDlgItemInt(hwnd, TOTAL, PrecioFinal, FALSE);
						}
						else { //SI NO SON LOS BOLETOS PARES ENTRA EN EL ELSE
							subT = (2 * 750) + 1500; //SON DOS BOLETOS AL PRECIO DE 1 + 1500 DEL QUE NO SE LE APLICA EL DESCUENTO
							IVACalculado = subT * 0.16;
							PrecioFinal = IVACalculado + subT;
							SetDlgItemInt(hwnd, SUBTOTAL, subT, FALSE);
							SetDlgItemInt(hwnd, IVA, IVACalculado, FALSE);
							SetDlgItemInt(hwnd, TOTAL, PrecioFinal, FALSE);
						}
						
					}
					if (DescuentoLong == 2) {//SI SE SELECCIONO 10% DE DESCUENTO EN EL CB REALIZARA ESTA OPERACION
						subT = nBoletos * 1500;
						IVACalculado = subT * 0.16;
						PrecioFinal = IVACalculado + (subT*0.90);
						SetDlgItemInt(hwnd, SUBTOTAL, subT, TRUE);
						SetDlgItemInt(hwnd, IVA, IVACalculado, TRUE);
						SetDlgItemInt(hwnd, TOTAL, PrecioFinal, TRUE);
					}
				}
			}

			if (LOWORD(wParam) == BTNCOMPRAR || LOWORD(wParam) == BN_CLICKED) {
				bool validardatos = true;
				bool validarboletos = true;

				HWND hEvento = GetDlgItem(hwnd, EVENTOCB);
				char EventoSelecc[20];
				GetWindowText(hEvento, EventoSelecc, sizeof(EventoSelecc));
				int EventoLong = SendMessage(hEvento, CB_GETCURSEL, 0, 0); //EXTRAE EL TEXTO DEL COMBOBOX
				if (EventoLong== CB_ERR) //SI NO SE SELECCIONO NADA LANZARA UNA ADVERTENCIA
				{
					MessageBox(hwnd, "DEBES SELECCIONAR UN EVENTO", "ADVERTENCIA", MB_OK | MB_ICONWARNING);
					validardatos = false;
					break;
				}
				
				HWND CantidadBoletos = GetDlgItem(hwnd, NUMBOLETOS);
				char Boletos[20];
				GetWindowText(CantidadBoletos, Boletos, sizeof(Boletos));
				int NumBoletosLong = GetWindowTextLength(CantidadBoletos);
				//VALIDA QUE SE SELECCIONE UNA CANTIDAD DE BOLETOS
				if (NumBoletosLong == 0) {
					MessageBox(hwnd, "SELECCIONE LA CANTIDAD DE BOLETOS", "ADVERTENCIA", MB_OK | MB_ICONWARNING);
					break;
				}
				for (int i = 0; i < NumBoletosLong; i++) {
					if (isalpha(Boletos[i])) //BUSCA LETRAS CARACTER POR CARACTER
					{
						validarboletos = false; //SI HAYA UNA LETRA EL BOOL SERA FALSO Y NO DEJARA GUARDAR LA INFORMACION
						validardatos = false;
					}
					if (isdigit(Boletos[i])) { //VALIDA QUE HAYA NUMEROS
						validarboletos = true;
						validardatos = true;
					}
				}//VALIDA QUE SOLO SE ACEPTEN NUMEROS
				if (validarboletos == false) {
					MessageBox(hwnd, "SOLO SE ACEPTAN NUMEROS EN LA CANTIDAD DE BOLETOS", "ERROR", MB_OK | MB_ICONERROR);
					break;
				}

				HWND hPromocion = GetDlgItem(hwnd, DESCUENTO);
				char hDes[20];
				GetWindowText(hPromocion, hDes, sizeof(hDes));
				int DescuentoLong = SendMessage(hPromocion, CB_GETCURSEL, 0, 0);
				if (DescuentoLong == CB_ERR) //VALIDAR QUE SE ESCOJA UN DESCUENTO
				{
					MessageBox(hwnd, "SELECCIONA SI QUIERES APLICAR UN DESCUENTO", "ADVERTENCIA", MB_OK | MB_ICONWARNING);
					validardatos = false;
					break;
				}

				HWND hTotal = GetDlgItem(hwnd, TOTAL);
				char TotalPagar[20];
				GetWindowText(hTotal, TotalPagar, sizeof(TotalPagar));
				int TotalLong = GetWindowTextLength(hTotal);
				//VALIDA QUE SI EL TOTAL NO TIENE NADA ES POR QUE NO SE HA CALCULADO EL TOTAL
				if (TotalLong == 0) {
					MessageBox(hwnd, "CALCULE EL PRECIO DE TODO ANTES DE REALIZAR EL PAGO", "ADVERTENCIA", MB_OK | MB_ICONWARNING);
					validardatos = false;
					break;
				}

				
				if (validardatos == true) { //SI TODOS LOS DATOS ESTAN CORRECTOS SE GUARDA LA INFORMACION
					nodo_eventos* compraticket = new nodo_eventos();
					
					if (EventoLong == 0) //VALIDAR QUE EVENTO SE ESCOGIO PARA COLOCAR LA FECHA
					{
						char Efecha[] = "05/12/2023"; //FECHA DE DUKI
						strcpy_s(compraticket->fecha, Efecha);
					}
					if (EventoLong == 1)
					{
						char Efecha[] = "08/12/2023"; //FECHA DE LUIS MIGUEL
						strcpy_s(compraticket->fecha, Efecha);
					}
					if (EventoLong == 2)
					{
						char Efecha[] = "15/12/2023"; //FECHA DE BZRP
						strcpy_s(compraticket->fecha, Efecha);
					}
					if (EventoLong == 3)
					{
						char Efecha[] = "21/12/2023"; //FECHA DE ARIANA GRANDE
						strcpy_s(compraticket->fecha, Efecha);
					}
					if (EventoLong == 4)
					{
						char Efecha[] = "28/12/2023"; //FECHA DE EMINEM
						strcpy_s(compraticket->fecha, Efecha);
					}
					//COPIA TODOS LOS CHARS DONDE SE ALMACENA LA INFORMACION DE LOS EDITCONTROL
					//Y MANDAMOS LLAMAR A LA FUNCION DE AGREGAR PARA GUARDAR LA INFORMACION EN LA ESTRUCTURA DE NODOS
					strcpy_s(compraticket->evento, EventoSelecc);
					strcpy_s(compraticket->numboletos, Boletos);
					strcpy_s(compraticket->precio, TotalPagar);
					agregar(compraticket);
					MessageBox(hwnd, "COMPRA EXITOSA", "CONFIRMACION", MB_OK);
					EndDialog(hwnd, 0);
					DialogBox(hInst, MAKEINTRESOURCE(MENU), hwnd, VMENUOPCIONES);
				}
			}
		}break;
	}
	return FALSE;
}

//VENTANA HISTORIAL DE COMPRAS
LRESULT CALLBACK VHISTORIALDECOMPRAS(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_INITDIALOG: {
			int idx = 0;
			auxiliar = lista_eventos;
			HWND hHistorial = GetDlgItem(hwnd, LISTAHISTORIALCOMPRA);
			
			if (auxiliar == NULL) { //SI NO HAY NINGUNA COMPRA REGISTRADA LANZARA ESTE MENSAJE EN EL LISTBOX
				SendMessage(hHistorial, LB_ADDSTRING, NULL, (LPARAM)"NO SE HA REGISTRADO NINGUNA COMPRA");
			}
			else {
				//SI HAY COMPRAS, MIENTRAS QUE NO SEA EL FIN DE LA LISTA, QUE MUESTRE TODAS LAS COMPRAS REALIZADAS
				while (auxiliar != NULL) {
					SendMessage(hHistorial, LB_INSERTSTRING, idx, (LPARAM)"Fecha:");
					SendMessage(hHistorial, LB_INSERTSTRING, idx + 1, (LPARAM)auxiliar->fecha);

					SendMessage(hHistorial, LB_INSERTSTRING, idx + 2, (LPARAM)"Evento:");
					SendMessage(hHistorial, LB_INSERTSTRING, idx + 3, (LPARAM)auxiliar->evento);

					SendMessage(hHistorial, LB_INSERTSTRING, idx + 4, (LPARAM)"Cantidad de boletos comprados:");
					SendMessage(hHistorial, LB_INSERTSTRING, idx + 5, (LPARAM)auxiliar->numboletos);

					SendMessage(hHistorial, LB_INSERTSTRING, idx + 6, (LPARAM)"Total:");
					SendMessage(hHistorial, LB_INSERTSTRING, idx + 7, (LPARAM)auxiliar->precio);

					SendMessage(hHistorial, LB_INSERTSTRING, idx + 8, (LPARAM)"\n");
					idx=idx+9;
					auxiliar = auxiliar->sig;
				}
			}
		}break;
		case WM_COMMAND: {
			if (LOWORD(wParam) == VOLVERMENU2 || LOWORD(wParam) == BN_CLICKED) {
				EndDialog(hwnd, 0);
				DialogBox(hInst, MAKEINTRESOURCE(MENU), hwnd, VMENUOPCIONES); //VOLVEMOS AL MENU
			}
		}break;
	}
	return FALSE;
}

//FUNCION PARA AGREGAR COMPRAS
void agregar(nodo_eventos* auxiliar) {
	auxiliar->ant = 0;
	auxiliar->sig = 0;
	if (lista_eventos == 0) { //SI LA LISTA DE EVENTOS ESTA VACIA, EL PRIMER NODO INGRESADO SERA EL PRIMERO DE LA LISTA
		lista_eventos = auxiliar;
	}
	else // SI NO ESTA VACIA, SE IRAN AGREGANDO LOS NODOS QUE SE VAYAN INGRESANDO
	{
		ultimo->sig = auxiliar;
		auxiliar->ant = ultimo;
	}
	ultimo = auxiliar;
}