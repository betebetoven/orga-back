
# Project Backend

## Overview

This backend serves as the computational and data handling layer for a graphical interface application. It processes grid data received from a frontend, performs various transformations and calculations, and returns useful metadata about the grid content.
## Frontend Repo Link
[Frontend Repo Link](https://github.com/betebetoven/orgra-front)

## Technologies Used

- FastAPI: Asynchronous web framework for building APIs.
- Pydantic: Data validation and settings management using Python type annotations.
- Uvicorn: ASGI server for hosting the FastAPI application.

## Features

- **Receive Grid Data**: Accepts JSON formatted grid data, processes it, and logs various characteristics of the data.
- **Figure Transformation**: Converts symbolic figure representations into designated internal codes.
- **Coordinates Extraction**: Identifies and lists the coordinates of cells containing figures.
- **Content Analysis**: Generates a detailed matrix describing the content of each cell.
- **Binary Matrix Generation**: Produces a binary matrix indicating the presence of figures in the grid.

## Setup

### Requirements

- Python 3.7+
- FastAPI
- Uvicorn
- Pydantic

### Installation

Clone the repository and navigate into the project directory:

```bash
git clone https://github.com/betebetoven/orga-back
cd orfa-final-backend
```

Install the required packages:

```bash
pip install fastapi uvicorn pydantic
```

### Running the Server

To run the server, use the following command:

```bash
uvicorn main:app --reload --port 8080
```

This command will start the server on `localhost` with port `8080`, and the `--reload` option enables auto-reload on code changes.

### Using the Application

1. **Start the server** as described above.
2. **Send requests** to `http://localhost:8080/receive-matrix/` using an appropriate JSON payload that matches the `GridData` schema.
3. **View responses** and logs in the terminal to understand how the data is being processed and responded to.

## API Endpoints

### POST `/receive-matrix/`

- **Purpose**: Receives grid data as JSON.
- **Payload**: JSON object conforming to the `GridData` model.
- **Response**: Confirmation message and any relevant data processing results.

## Development

For development, you can use the `--reload` flag with `uvicorn` to enable live reloading. Make sure to write unit tests for new features and integrate continuous integration tools to automate testing.

## Contribution

Contributions are welcome. Please fork the repository, make your changes, and submit a pull request. For major changes, please open an issue first to discuss what you would like to change.

Ensure to update tests as appropriate.

## License

[MIT](https://choosealicense.com/licenses/mit/)
