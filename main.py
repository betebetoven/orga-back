from fastapi import FastAPI, Request, HTTPException
from fastapi.responses import JSONResponse,  StreamingResponse
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
from typing import List, Optional

app = FastAPI()
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)
class Cell(BaseModel):
    figure: Optional[str]
    color: Optional[str]

class GridData(BaseModel):
    grid: List[List[Cell]]

@app.post("/receive-matrix/")
async def receive_matrix(data: GridData):
    print("Received grid data:")
    for row in data.grid:
        print(row)
    return {"message": "Grid data received successfully!"}

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8080)
