from fastapi import FastAPI, Request, HTTPException
from fastapi.responses import JSONResponse,  StreamingResponse
from fastapi.middleware.cors import CORSMiddleware
from modelos_base import GridData
from funciones import get_figure_coordinates, get_figure_matrix, get_figure_content, get_data, get_big_integer
import serial, time
import json

PuertoSerie  = serial.Serial('COM5', 9600)
time.sleep(2)

app = FastAPI()
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

    



@app.post("/receive-matrix/")
async def receive_matrix(data: GridData):
    coordenadas = get_figure_coordinates(data)
    matrizBinaria = get_figure_matrix(coordenadas, data.grid)
    matriz_contenido = get_figure_content(data)
    print(matriz_contenido)
    #matriz binaria a texto lineal
    textoBinario = ""
    for i in matrizBinaria:
        for j in i:
            textoBinario += str(j)
    print(textoBinario)
    big_num = get_big_integer(data)
    print(big_num)
    #set big num as bytes
    texto_binario_para_enviar = textoBinario + "\n"
    big_num = big_num + "\n"
    big_num = big_num.encode()
    texto_binario_para_enviar = texto_binario_para_enviar.encode()
    #PuertoSerie.write(big_num)
    PuertoSerie.write(texto_binario_para_enviar)
    
    return JSONResponse(content={"message": "Data sent successfully"})



@app.post("/receive-matrix-send-ram/")
async def receive_matrix_ram(data: GridData):
    coordenadas = get_figure_coordinates(data)
    matrizBinaria = get_figure_matrix(coordenadas, data.grid)
    matriz_contenido = get_figure_content(data)
    print(matriz_contenido)
    #matriz binaria a texto lineal
    textoBinario = ""
    for i in matrizBinaria:
        for j in i:
            textoBinario += str(j)
    print(textoBinario)
    big_num = get_big_integer(data)
    print(big_num)
    #set big num as bytes
    texto_binario_para_enviar = textoBinario + "\n"
    big_num = big_num + "\n"
    big_num = big_num.encode()
    PuertoSerie.write(texto_binario_para_enviar)
    
    return JSONResponse(content={"message": "Data sent successfully"})

#endpoint that is a get and when is called makes a puerto serie.write reset
@app.get("/reset/")
async def reset():
    PuertoSerie.write(b"q")
    print("reset")
    return JSONResponse(content={"message": "Reset sent successfully"})

    


if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8080)
