# Personal Blogging Platform RESTful API

This project implements a simple RESTful API for a personal blogging platform.  The API provides basic CRUD (Create, Read, Update, Delete) operations for blog posts.

## API Endpoints

All endpoints use standard HTTP methods and return JSON-formatted responses.

### 1. Create a New Blog Post

**Method:** `POST`

**Endpoint:** `/posts`

**Request Body (JSON):**

```json
{
  "title": "My First Blog Post",
  "content": "This is the content of my first blog post.",
  "category": "Technology",
  "tags": ["Tech", "Programming"]
}
```
**Response 201 (Created):**

```json
{
  "id": 1,
  "title": "My First Blog Post",
  "content": "This is the content of my first blog post.",
  "category": "Technology",
  "tags": ["Tech", "Programming"],
  "createdAt": "2024-10-27T12:00:00Z",
  "updatedAt": "2024-10-27T12:00:00Z"
}
```

**Response 400 (Bad Request):** `JSON with error details.`

### 2. Update an Existing Blog Post

**Method:** `PUT`

**Endpoint:** `/posts/{id}`

**Request Body (JSON):**

```json
{
  "title": "Updated Post Title",
  "content": "New and improved content!",
  "category": "Science",
  "tags": ["Science", "Physics"]
}
```

**Response 200 (OK):**

```json
{
  "id": 1,
  "title": "Updated Post Title",
  "content": "New and improved content!",
  "category": "Science",
  "tags": ["Science", "Physics"],
  "createdAt": "2024-10-27T12:00:00Z",
  "updatedAt": "2024-10-27T12:30:00Z"
}
```

**Response 400 (Bad Request):** `JSON with error details.`

**Response 404 (Not Found):** `If the post with the given ID doesn’t exist.`

### 3. Delete a Blog Post

**Method:** `DELETE`

**Endpoint:** `/posts/{id}`

**Response 204 (No Content):** `Successful deletion (empty response body).`

**Response 404 (Not Found):** `If the post with the given ID doesn’t exist.`

### 4. Get a Single Blog Post

**Method:** `GET`

**Endpoint:** `/posts/{id}`

**Response** 200 (OK): `(Same JSON structure as in Create and Update)`

**Response 404 (Not Found):** `If the post with the given ID doesn’t exist.`

### 5. Get All Blog Posts

**Method:** `GET`

**Endpoint:** `/posts`

**Response 200 (OK):**

```json
[
  {
    "id": 1,
    "title": "Post 1",
    "content": "...",
    "category": "...",
    "tags": [...],
    "createdAt": "YYYY-MM-DDTHH:mm:ssZ",
    "updatedAt": "YYYY-MM-DDTHH:mm:ssZ"
  },
  {
    "id": 2,
    "title": "Post 2",
    // ... more posts
  }
]

```

### Contribution Guidelines
Contributions are welcome! Please open an issue to discuss proposed changes before submitting a pull request.  Pull requests should include clear descriptions of the changes made.

I am particularly interested in improvements related to:

* API efficiency and performance optimizations (especially for large datasets)
* Enhanced error handling and informative error responses (using appropriate HTTP status codes)
* Improved API design and adherence to RESTful principles
* Robust input validation and sanitization to prevent security vulnerabilities
* Expansion of features (e.g., user authentication, commenting, etc.)  (See our roadmap for ideas)

This project aims to provide a simple and efficient RESTful API for a personal blogging platform.  Feel free to explore, experiment, and contribute to make it even more useful.

## Disclaimer:
This project is for educational and demonstration purposes only.

## The terms of the task can be viewed on the website:
https://roadmap.sh/projects/blogging-platform-api
