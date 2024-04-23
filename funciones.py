from modelos_base import GridData
#devuelve las coordenadas de las figuras
def get_figure_coordinates(data: GridData):
    coordinates = []
    for i in range(len(data.grid)):
        for j in range(len(data.grid[i])):
            if data.grid[i][j].figure:
                coordinates.append((i, j))
    return coordinates

def get_figure_content(data: GridData):
    figure_matrix = [[0 for _ in range(len(data.grid[0]))] for _ in range(len(data.grid))]
    for i in range(len(data.grid)):
        for j in range(len(data.grid[i])):
            cell = data.grid[i][j]
            if cell.figure:
                figure_matrix[i][j] = [cell.figure,cell.color]
    return figure_matrix
#devuelve matriz binaria
def get_figure_matrix(coordinates, grid):
    figure_matrix = [[0 for _ in range(len(grid[0]))] for _ in range(len(grid))]
    for i, j in coordinates:
        figure_matrix[i][j] = 1
    return figure_matrix
