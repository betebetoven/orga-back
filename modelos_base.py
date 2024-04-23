from pydantic import BaseModel, validator
from typing import List, Optional
class Cell(BaseModel):
    figure: Optional[str]
    color: Optional[str]

    @validator('figure', pre=True, always=True)
    def transform_figure(cls, v):
        if v == "△":
            return "t"
        elif v == "☆":
            return "s"
        return v

class GridData(BaseModel):
    grid: List[List[Cell]]
