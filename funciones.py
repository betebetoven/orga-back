from modelos_base import GridData
#devuelve las coordenadas de las figuras
dict_figura_a_int = {
    "t": 1,
    "s": 2,
    "x": 3,
    "o": 4,
}
dict_color_a_int = {
    "cyan": 1,
    "yellow": 2,
    "magenta": 3,
    "negro": 4,
}
def get_figure_coordinates(data: GridData):
    coordinates = []
    for i in range(len(data.grid)):
        for j in range(len(data.grid[i])):
            if data.grid[i][j].figure:
                coordinates.append((i, j))
    return coordinates
#devuelve matriz basica para arduino
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


def get_data(data: GridData):
    return data.grid

def get_big_integer(data: GridData):
    figure_content = get_figure_content(data)
    big_integer = ""
    for row in figure_content:
        for cell in row:
            if cell == 0:
                big_integer += "0"
            else:
                figure = dict_figura_a_int[cell[0]]
                #color = dict_color_a_int[cell[1]]
                big_integer += str(figure) 
            #big_integer += "5"
    return str(big_integer)