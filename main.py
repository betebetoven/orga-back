from fastapi import FastAPI, Request, HTTPException
from fastapi.responses import JSONResponse,  StreamingResponse
from fastapi.middleware.cors import CORSMiddleware
from modelos_base import GridData
from funciones import get_figure_coordinates, get_figure_matrix

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
    print("Received grid data:")
    for row in data.grid:
        print(row)
    matriz_coordenadas = get_figure_coordinates(data)
    print("Coordinates of the cells with figures:", matriz_coordenadas)
    matriz_binaria = get_figure_matrix(matriz_coordenadas, data.grid)
    print("Binary matrix of the figure:", matriz_binaria)
    return {"message": "Grid data received successfully!"}



if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8080)
