/*                         _
 *   ___  __ _  __ _ _   _(_)
 *  / __|/ _` |/ _` | | | | |
 *  \__ \ (_| | (_| | |_| | |
 *  |___/\__,_|\__, |\__,_|_|
 *             |___/
 *
 *   –– cross-platform library which helps to develop web servers or frameworks.
 *
 * Copyright (c) 2016-2018 Silvio Clecio <silvioprog@gmail.com>
 *
 * This file is part of Sagui library.
 *
 * Sagui library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sagui library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Sagui library.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \mainpage
 * \li \ref sg_api
 */

/** \defgroup sg_api API reference
 * The API reference grouped by feature.
 **/

#ifndef SAGUI_H
#define SAGUI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#ifndef SG_EXTERN
# ifdef _WIN32
#  ifdef BUILDING_LIBSAGUI
#   define SG_EXTERN __declspec(dllexport) extern
#  else
#   define SG_EXTERN __declspec(dllimport) extern
#  endif
# else
#  define SG_EXTERN extern
# endif
#endif

#ifndef __SG_UNUSED
#define __SG_UNUSED __attribute__((unused))
#endif

#ifndef __SG_MALLOC
#define __SG_MALLOC __attribute__((malloc))
#endif

#ifndef __SG_FORMAT
#define __SG_FORMAT(...) __attribute__((format(printf, __VA_ARGS__)))
#endif

#define SG_VERSION_MAJOR 1
#define SG_VERSION_MINOR 2
#define SG_VERSION_PATCH 0
#define SG_VERSION_HEX ((SG_VERSION_MAJOR << 16) | (SG_VERSION_MINOR <<  8) | (SG_VERSION_PATCH))

#define SG_ERR_SIZE 256

/**
 * \ingroup sg_api
 * \defgroup sg_utils Utilities
 * All utility functions of the library.
 * \{
 */

/**
 * Callback signature used by functions that handle errors.
 * \param[out] cls User-defined closure.
 * \param[out] err Error message.
 */
typedef void (*sg_err_cb)(void *cls, const char *err);

/**
 * Callback signature used by functions that write streams.
 * \param[out] handle Stream handle.
 * \param[out] offset Current stream offset.
 * \param[out] buf Current buffer to be written.
 * \param[out] size Size of the current buffer to be written.
 * \return Total written buffer.
 */
typedef size_t (*sg_write_cb)(void *handle, uint64_t offset, const char *buf, size_t size);

/**
 * Callback signature used by functions that read streams.
 * \param[out] handle Stream handle.
 * \param[out] offset Current stream offset.
 * \param[out] buf Current read buffer.
 * \param[out] size Size of the current read buffer.
 * \return Total read buffer.
 */
typedef ssize_t (*sg_read_cb)(void *handle, uint64_t offset, char *buf, size_t size);

/**
 * Callback signature used by functions that free streams.
 * \param[out] handle Stream handle.
 */
typedef void (*sg_free_cb)(void *handle);

/**
 * Callback signature used by functions that save streams.
 * \param[out] handle Stream handle.
 * \param[out] overwritten Overwrite an already existed stream.
 * \retval 0 - Success.
 * \retval E<ERROR> - User-defined error to abort the saving.
 */
typedef int (*sg_save_cb)(void *handle, bool overwritten);

/**
 * Callback signature used by functions that save streams. It allows to specify the destination file path.
 * \param[out] handle Stream handle.
 * \param[out] path Absolute path to store the stream.
 * \param[out] overwritten Overwrite an already existed stream.
 * \retval 0 - Success.
 * \retval E<ERROR> - User-defined error to abort the saving.
 */
typedef int (*sg_save_as_cb)(void *handle, const char *path, bool overwritten);

/**
 * Returns the library version number.
 * \return Library version packed into a single integer.
 */
SG_EXTERN unsigned int sg_version(void);

/**
 * Returns the library version number as string.
 * \return Library version packed into a null-terminated string.
 */
SG_EXTERN const char *sg_version_str(void);

/**
 * Allocates a new zero-initialize memory space.
 * \param[in] size Memory size to be allocated.
 * \return Pointer of the zero-initialized allocated memory.
 * \retval NULL If size is `0` or no memory space.
 */
SG_EXTERN void *sg_alloc(size_t size)
__SG_MALLOC;

/**
 * Reallocates an existing memory block.
 * \param[in,out] ptr Pointer of the memory to be reallocated.
 * \param[in] size Memory size to be reallocated.
 * \return Pointer of the reallocated memory.
 * \note Equivalent to [realloc(3)](https://linux.die.net/man/3/realloc).
 */
SG_EXTERN void *sg_realloc(void *ptr, size_t size)
__SG_MALLOC;

/**
 * Frees a memory space previously allocated by #sg_alloc() or #sg_realloc().
 * \param[in] ptr Pointer of the memory to be freed.
 */
SG_EXTERN void sg_free(void *ptr);

/**
 * Returns string describing an error number.
 * \param[in] errnum Error number.
 * \param[in,out] errmsg Pointer of a string to store the error message.
 * \param[in] errlen Length of the error message.
 * \return Pointer to \pr{str}.
 */
SG_EXTERN char *sg_strerror(int errnum, char *errmsg, size_t errlen);

/**
 * Checks if a string is a HTTP post method.
 * \param[in] method Null-terminated string.
 * \return `true` if \pr{method} is `POST`, `PUT`, `DELETE` or `OPTIONS`.
 */
SG_EXTERN bool sg_is_post(const char *method);

/**
 * Extracts the entry-point of a path or resource. For example, given a path
 * `/api1/customer`, the part considered as entry-point is `/api1`.
 * \param path Entry-point as null-terminated string.
 * \retval NULL If no memory space is available.
 */
SG_EXTERN char *sg_extract_entrypoint(const char *path);

/**
 * Returns the system temporary directory.
 * \return Temporary directory as null-terminated string.
 * \retval NULL If no memory space is available.
 */
SG_EXTERN char *sg_tmpdir(void);

/** \} */

/**
 * \ingroup sg_api
 * \defgroup sg_str String
 * String handle and its related functions.
 * \{
 */

/**
 * Handle for the string structure used to represent a HTML body, POST payload and more.
 * \struct sg_str
 */
struct sg_str;

/**
 * Creates a new zero-initialized string handle.
 * \return String handle.
 * \warning It exits the application if called when no memory space is available.
 */
SG_EXTERN struct sg_str *sg_str_new(void)
__SG_MALLOC;

/**
 * Frees the string handle previously allocated by #sg_str_new().
 * \param[in] str Pointer of the string handle to be freed.
 */
SG_EXTERN void sg_str_free(struct sg_str *str);

/**
 * Writes a null-terminated string to the string handle \pr{str}. All strings previously written are kept.
 * \param[in] str String handle.
 * \param[in] val String to be written.
 * \param[in] len Length of the string to be written.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 */
SG_EXTERN int sg_str_write(struct sg_str *str, const char *val, size_t len);

/**
 * Prints a null-terminated formatted string from the argument list to the string handle \pr{str}.
 * \param[in] str String handle.
 * \param[in] fmt Formatted string (following the same [`printf()`](https://linux.die.net/man/3/printf) format
 *  specification).
 * \param[in] ap Arguments list (handled by
 *  [`va_start()`](https://linux.die.net/man/3/va_start)/[`va_end()`](https://linux.die.net/man/3/va_end)).
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 */
SG_EXTERN int sg_str_printf_va(struct sg_str *str, const char *fmt, va_list ap);

/**
 * Prints a null-terminated formatted string to the string handle \pr{str}. All strings previously written are kept.
 * \param[in] str String handle.
 * \param[in] fmt Formatted string (following the same [`printf()`](https://linux.die.net/man/3/printf) format
 *  specification).
 * \param[in] ... Additional arguments (following the same [`printf()`](https://linux.die.net/man/3/printf) arguments
 *  specification).
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 */
SG_EXTERN int sg_str_printf(struct sg_str *str, const char *fmt, ...)
__SG_FORMAT(2, 3);

/**
 * Returns the null-terminated string content from the string handle \pr{str}.
 * \param[in] str String handle.
 * \return Content as null-terminated string.
 * \retval NULL If the \pr{str} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN const char *sg_str_content(struct sg_str *str);

/**
 * Returns the total string length from the handle \pr{str}.
 * \param[in] str String handle.
 * \return Total string length.
 * \retval EINVAL - Invalid argument.
 */
SG_EXTERN size_t sg_str_length(struct sg_str *str);

/**
 * Cleans all existing content in the string handle \pr{str}.
 * \param[in] str String handle.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 */
SG_EXTERN int sg_str_clear(struct sg_str *str);

/** \} */

/**
 * \ingroup sg_api
 * \defgroup sg_strmap String map
 * String map handle and its related functions.
 * \{
 */

/**
 * Handle for hash table that maps name-value pairs. It is useful to represent posting fields, query-string parameter,
 * client cookies and more.
 * \struct sg_strmap
 */
struct sg_strmap;

/**
 * Callback signature used by #sg_strmap_iter() to iterate pairs of strings.
 * \param[out] cls User-defined closure.
 * \param[out] pair Current iterated pair.
 * \retval 0 - Success.
 * \retval E<ERROR> - User-defined error to stop pairs iteration.
 */
typedef int (*sg_strmap_iter_cb)(void *cls, struct sg_strmap *pair);

/**
 * Callback signature used by #sg_strmap_sort() to sort pairs of strings.
 * \param[out] cls User-defined closure.
 * \param[out] pair_a Current left pair (A).
 * \param[out] pair_b Current right pair (B).
 * \retval -1 - A < B.
 * \retval 0 - A == B.
 * \retval 1 - A > B.
 */
typedef int (*sg_strmap_sort_cb)(void *cls, struct sg_strmap *pair_a, struct sg_strmap *pair_b);

/**
 * Returns a name from the \pr{pair}.
 * \param[in] pair Pair of name-value.
 * \return Name as null-terminated string.
 * \retval NULL If the \pr{pair} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN const char *sg_strmap_name(struct sg_strmap *pair);

/**
 * Returns a value from the \pr{pair}.
 * \param[in] pair Pair of name-value.
 * \return Value as null-terminated string.
 * \retval NULL If the \pr{pair} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN const char *sg_strmap_val(struct sg_strmap *pair);

/**
 * Adds a pair of name-value to the string \pr{map}.
 * \param[in,out] map Pairs map pointer to add a new pair.
 * \param[in] name Pair name.
 * \param[in] val Pair value.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \note It cannot check if a name already exists in a pair added to the \pr{map}, then the uniqueness must be managed
 * by the application.
 * \warning It exits the application if called when no memory space is available.
 */
SG_EXTERN int sg_strmap_add(struct sg_strmap **map, const char *name, const char *val);

/**
 * Sets a pair of name-value to the string \pr{map}.
 * \param[in,out] map Pairs map pointer to set a new pair.
 * \param[in] name Pair name.
 * \param[in] val Pair value.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \note If a name already exists in a pair previously added into the \pr{map}, then the function replaces its value,
 * otherwise it is added as a new pair.
 * \warning It exits the application if called when no memory space is available.
 */
SG_EXTERN int sg_strmap_set(struct sg_strmap **map, const char *name, const char *val);

/**
 * Finds a pair by name.
 * \param[in] map Pairs map.
 * \param[in] name Name to find the pair.
 * \param[in,out] pair Pointer of the variable to store the found pair.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \retval ENOENT - Pair not found.
 */
SG_EXTERN int sg_strmap_find(struct sg_strmap *map, const char *name, struct sg_strmap **pair);

/**
 * Gets a pair by name and returns the value.
 * \param[in] map Pairs map.
 * \param[in] name Name to get the pair.
 * \return Pair value as null-terminated string.
 * \retval NULL If \pr{map} or \pr{name} is null or pair is not found.
 */
SG_EXTERN const char *sg_strmap_get(struct sg_strmap *map, const char *name);

/**
 * Removes a pair by name.
 * \param[in] map Pointer to the pairs map.
 * \param[in] name Name to find and then remove the pair.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \retval ENOENT - Pair already removed.
 */
SG_EXTERN int sg_strmap_rm(struct sg_strmap **map, const char *name);

/**
 * Iterates over pairs map.
 * \param[in] map Pairs map.
 * \param[in] cb Callback to iterate the pairs.
 * \param[in,out] cls User-specified value.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \return Callback result when it is different from `0`.
 */
SG_EXTERN int sg_strmap_iter(struct sg_strmap *map, sg_strmap_iter_cb cb, void *cls);

/**
 * Sorts the pairs map.
 * \param[in,out] map Pointer to the pairs map.
 * \param[in] cb Callback to sort the pairs.
 * \param[in,out] cls User-specified value.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 */
SG_EXTERN int sg_strmap_sort(struct sg_strmap **map, sg_strmap_sort_cb cb, void *cls);

/**
 * Counts the total pairs in the map.
 * \param[in] map Pairs map.
 * \return Total of pairs.
 * \retval 0 If the list is empty or null.
 */
SG_EXTERN unsigned int sg_strmap_count(struct sg_strmap *map);

/**
 * Returns the next pair in the map.
 * \param[in,out] next Pointer to the next pair.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 */
SG_EXTERN int sg_strmap_next(struct sg_strmap **next);

/**
 * Cleans the entire map.
 * \param[in,out] map Pointer to the pairs map.
 */
SG_EXTERN void sg_strmap_cleanup(struct sg_strmap **map);

/** \} */

/**
 * \ingroup sg_api
 * \defgroup sg_httpsrv HTTP server
 * Fast event-driven HTTP server.
 * \{
 */

/**
 * Handle for the HTTP basic authentication.
 * \struct sg_httpauth
 */
struct sg_httpauth;

/**
 * Handle for the upload handling. It is used to represent a single upload or a list of uploads.
 * \struct sg_httpupld
 */
struct sg_httpupld;

/**
 * Handle for the request handling. It contains headers, cookies, query-string, fields, payloads, uploads and other
 * data sent by the client.
 * \struct sg_httpreq
 */
struct sg_httpreq;

/**
 * Handle for the response handling. It dispatches headers, contents, binaries, files and other data to the client.
 * \struct sg_httpres
 */
struct sg_httpres;

/**
 * Handle for the fast event-driven HTTP server.
 * \struct sg_httpsrv
 */
struct sg_httpsrv;

/**
 * Callback signature used to grant or deny the user access to the server resources.
 * \param[out] cls User-defined closure.
 * \param[out] auth Authentication handle.
 * \param[out] req Request handle.
 * \param[out] res Response handle.
 * \retval true Grants the user access.
 * \retval false Denies the user access.
 */
typedef bool (*sg_httpauth_cb)(void *cls, struct sg_httpauth *auth, struct sg_httpreq *req, struct sg_httpres *res);

/**
 * Callback signature used to handle uploaded files and/or fields.
 * \param[out] cls User-defined closure.
 * \param[in,out] handle Stream handle pointer.
 * \param[out] dir Directory to store the uploaded files.
 * \param[out] field Posted field.
 * \param[out] name Uploaded file name.
 * \param[out] mime Uploaded file content-type (e.g.: `text/plain`, `image/png`, `application/json` etc.).
 * \param[out] encoding Uploaded file transfer-encoding (e.g.: `chunked`, `deflate`, `gzip` etc.).
 * \retval 0 - Success.
 * \retval E<ERROR> - User-defined error to refuse the upload.
 */
typedef int (*sg_httpupld_cb)(void *cls, void **handle, const char *dir, const char *field, const char *name,
                              const char *mime, const char *encoding);

/**
 * Callback signature used to iterate uploaded files.
 * \param[out] cls User-defined closure.
 * \param[out] upld Current upload item.
 * \retval 0 - Success.
 * \retval E<ERROR> - User-defined error to stop list iteration.
 */
typedef int (*sg_httpuplds_iter_cb)(void *cls, struct sg_httpupld *upld);

/**
 * Callback signature used to handle requests and responses.
 * \param[out] cls User-defined closure.
 * \param[out] req Request handle.
 * \param[out] res Response handle.
 */
typedef void (*sg_httpreq_cb)(void *cls, struct sg_httpreq *req, struct sg_httpres *res);

/**
 * Sets the authentication protection space (realm).
 * \param[in] auth Authentication handle.
 * \param[in] realm Realm string.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \retval EALREADY - Realm already set.
 * \warning It exits the application if called when no memory space is available.
 */
SG_EXTERN int sg_httpauth_set_realm(struct sg_httpauth *auth, const char *realm);

/**
 * Gets the authentication protection space (realm).
 * \param[in] auth Authentication handle.
 * \return Realm as null-terminated string.
 * \retval NULL If \pr{auth} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN const char *sg_httpauth_realm(struct sg_httpauth *auth);

/**
 * Deny the authentication sending a justification to the user.
 * \param[in] auth Authentication handle.
 * \param[in] justification Justification message.
 * \param[in] content_type `Content-Type` of the justification.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \retval EALREADY - Already denied.
 */
SG_EXTERN int sg_httpauth_deny(struct sg_httpauth *auth, const char *justification, const char *content_type);

/**
 * Cancels the authentication loop while the user is trying to acess the server.
 * \param[in] auth Authentication handle.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 */
SG_EXTERN int sg_httpauth_cancel(struct sg_httpauth *auth);

/**
 * Returns the authentication user.
 * \param[in] auth Authentication handle.
 * \return User as null-terminated string.
 * \retval NULL If \pr{auth} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN const char *sg_httpauth_usr(struct sg_httpauth *auth);

/**
 * Returns the authentication password.
 * \param[in] auth Authentication handle.
 * \return Password as null-terminated string.
 * \retval NULL If \pr{auth} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN const char *sg_httpauth_pwd(struct sg_httpauth *auth);

/**
 * Iterates over all the upload items in the \pr{uplds} list.
 * \param[in] uplds Uploads list handle.
 * \param[in] cb Callback to iterate over upload items.
 * \param[in] cls User-defined closure.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \retval E<ERROR> - User-defined error to abort the list iteration.
 */
SG_EXTERN int sg_httpuplds_iter(struct sg_httpupld *uplds, sg_httpuplds_iter_cb cb, void *cls);

/**
 * Gets the next upload item starting from the first item pointer \pr{upld}.
 * \param[in,out] upld Next upload item starting from the first item pointer.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 */
SG_EXTERN int sg_httpuplds_next(struct sg_httpupld **upld);

/**
 * Counts the total upload items in the list \pr{uplds}.
 * \param[in] uplds Uploads list.
 * \return Total of items.
 * \retval 0 If the list is empty or null.
 */
SG_EXTERN unsigned int sg_httpuplds_count(struct sg_httpupld *uplds);

/**
 * Returns the stream handle of the upload handle \pr{upld}.
 * \param[in] upld Upload handle.
 * \return Stream handle.
 * \retval NULL If \pr{upld} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN void *sg_httpupld_handle(struct sg_httpupld *upld);

/**
 * Returns the directory of the upload handle \pr{upld}.
 * \param[in] upld Upload handle.
 * \return Upload directory as null-terminated string.
 * \retval NULL If \pr{upld} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN const char *sg_httpupld_dir(struct sg_httpupld *upld);

/**
 * Returns the field of the upload handle \pr{upld}.
 * \param[in] upld Upload handle.
 * \return Upload field as null-terminated string.
 * \retval NULL If \pr{upld} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN const char *sg_httpupld_field(struct sg_httpupld *upld);

/**
 * Returns the name of the upload handle \pr{upld}.
 * \param[in] upld Upload handle.
 * \return Upload name as null-terminated string.
 * \retval NULL If \pr{upld} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN const char *sg_httpupld_name(struct sg_httpupld *upld);

/**
 * Returns the MIME (content-type) of the upload.
 * \param[in] upld Upload handle.
 * \return Upload MIME as null-terminated string.
 * \retval NULL If \pr{upld} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN const char *sg_httpupld_mime(struct sg_httpupld *upld);

/**
 * Returns the encoding (transfer-encoding) of the upload.
 * \param[in] upld Upload handle.
 * \return Upload encoding as null-terminated string.
 * \retval NULL If \pr{upld} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN const char *sg_httpupld_encoding(struct sg_httpupld *upld);

/**
 * Returns the size of the upload.
 * \param[in] upld Upload handle.
 * \return Upload size into `uint64`. If \pr{upld} is null, sets the `errno` to `EINVAL`.
 */
SG_EXTERN uint64_t sg_httpupld_size(struct sg_httpupld *upld);

/**
 * Saves the uploaded file defining the destination path by upload name and directory.
 * \param[in] upld Upload handle.
 * \param[in] overwritten Overwrite upload file if it exists.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \retval EEXIST - File already exists (if \pr{overwritten} is `false`).
 * \retval EISDIR - Destination file is a directory.
 */
SG_EXTERN int sg_httpupld_save(struct sg_httpupld *upld, bool overwritten);

/**
 * Saves the uploaded file allowing to define the destination path.
 * \param[in] upld Upload handle.
 * \param[in] path Absolute destination path.
 * \param[in] overwritten Overwrite upload file if it exists.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \retval EEXIST - File already exists (if \pr{overwritten} is `true`).
 * \retval EISDIR - Destination file is a directory.
 */
SG_EXTERN int sg_httpupld_save_as(struct sg_httpupld *upld, const char *path, bool overwritten);

/**
 * Returns the client headers into #sg_strmap map.
 * \param[in] req Request handle.
 * \return Reference to the client headers map.
 * \retval NULL If \pr{req} is null and sets the `errno` to `EINVAL`
 * \note The headers map is automatically freed by the library.
 */
SG_EXTERN struct sg_strmap **sg_httpreq_headers(struct sg_httpreq *req);

/**
 * Returns the client cookies into #sg_strmap map.
 * \param[in] req Request handle.
 * \return Reference to the client cookies map.
 * \retval NULL If \pr{req} is null and sets the `errno` to `EINVAL`
 * \note The cookies map is automatically freed by the library.
 */
SG_EXTERN struct sg_strmap **sg_httpreq_cookies(struct sg_httpreq *req);

/**
 * Returns the query-string into #sg_strmap map.
 * \param[in] req Request handle.
 * \return Reference to the query-string map.
 * \retval NULL If \pr{req} is null and sets the `errno` to `EINVAL`
 * \note The query-string map is automatically freed by the library.
 */
SG_EXTERN struct sg_strmap **sg_httpreq_params(struct sg_httpreq *req);

/**
 * Returns the fields of a HTML form into #sg_strmap map.
 * \param[in] req Request handle.
 * \return Reference to the form fields map.
 * \retval NULL If \pr{req} is null and sets the `errno` to `EINVAL`
 * \note The form fields map is automatically freed by the library.
 */
SG_EXTERN struct sg_strmap **sg_httpreq_fields(struct sg_httpreq *req);

/**
 * Returns the HTTP version.
 * \param[in] req Request handle.
 * \return HTTP version as null-terminated string.
 * \retval NULL If \pr{req} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN const char *sg_httpreq_version(struct sg_httpreq *req);

/**
 * Returns the HTTP method.
 * \param[in] req Request handle.
 * \return HTTP method as null-terminated string.
 * \retval NULL If \pr{req} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN const char *sg_httpreq_method(struct sg_httpreq *req);

/**
 * Returns the path component.
 * \param[in] req Request handle.
 * \return Path component as null-terminated string.
 * \retval NULL If \pr{req} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN const char *sg_httpreq_path(struct sg_httpreq *req);

/**
 * Returns the posting payload into a #sg_str instance.
 * \param[in] req Request handle.
 * \return Instance of the payload.
 * \retval NULL If \pr{req} is null and sets the `errno` to `EINVAL`.
 * \note The form payload instance is automatically freed by the library.
 */
SG_EXTERN struct sg_str *sg_httpreq_payload(struct sg_httpreq *req);

/**
 * Checks if the client is uploading data.
 * \param[in] req Request handle.
 * \return `true` if the client is uploading data, `false` otherwise. If \pr{req} is null, sets the `errno` to `EINVAL`.
 */
SG_EXTERN bool sg_httpreq_is_uploading(struct sg_httpreq *req);

/**
 * Returns the list of the uploaded files.
 * \param[in] req Request handle.
 * \return List of the uploaded files.
 * \retval NULL If \pr{req} is null and sets the `errno` to `EINVAL`.
 * \note The uploads list is automatically freed by the library.
 */
SG_EXTERN struct sg_httpupld *sg_httpreq_uploads(struct sg_httpreq *req);

#ifdef SG_HTTPS_SUPPORT

/**
 * Returns the GnuTLS session handle.
 * \param[in] req Request handle.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 */
SG_EXTERN void *sg_httpreq_tls_session(struct sg_httpreq *req);

#endif

/**
 * Sets user data to the request handle.
 * \param[in] req Request handle.
 * \param[in] data User data pointer.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 */
SG_EXTERN int sg_httpreq_set_user_data(struct sg_httpreq *req, void *data);

/**
 * Gets user data from the request handle.
 * \param[in] req Request handle.
 * \return User data pointer.
 * \retval NULL If \pr{req} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN void *sg_httpreq_user_data(struct sg_httpreq *req);

/**
 * Returns the server headers into #sg_strmap map.
 * \param[in] res Response handle.
 * \return Reference to the server headers map.
 * \retval NULL If \pr{res} is null and sets the `errno` to `EINVAL`
 * \note The headers map is automatically freed by the library.
 */
SG_EXTERN struct sg_strmap **sg_httpres_headers(struct sg_httpres *res);

/**
 * Sets server cookie to the response handle.
 * \param[in] res Response handle.
 * \param[in] name Cookie name.
 * \param[in] val Cookie value.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \warning It exits the application if called when no memory space is available.
 */
SG_EXTERN int sg_httpres_set_cookie(struct sg_httpres *res, const char *name, const char *val);

/**
 * Sends a null-terminated string content to the client.
 * \param[in] res Response handle.
 * \param[in] val Null-terminated string.
 * \param[in] content_type `Content-Type` of the content.
 * \param[in] status HTTP status code.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \retval EALREADY - Operation already in progress.
 * \warning It exits the application if called when no memory space is available.
 */
#define sg_httpres_send(res, val, content_type, status) \
    sg_httpres_sendbinary((res), (void *) (val), ((val != NULL) ? strlen((val)) : 0), (content_type), (status))

/**
 * Sends a binary content to the client.
 * \param[in] res Response handle.
 * \param[in] buf Binary content.
 * \param[in] size Content size.
 * \param[in] content_type `Content-Type` of the content.
 * \param[in] status HTTP status code.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \retval EALREADY - Operation already in progress.
 * \warning It exits the application if called when no memory space is available.
 */
SG_EXTERN int sg_httpres_sendbinary(struct sg_httpres *res, void *buf, size_t size, const char *content_type,
                                    unsigned int status);

/**
 * Sends a file to the client.
 * \param[in] res Response handle.
 * \param[in] block_size Preferred block size for file loading.
 * \param[in] max_size Maximum allowed file size.
 * \param[in] filename Path of the file to be sent.
 * \param[in] rendered If `true` the file is rendered, otherwise downloaded.
 * \param[in] status HTTP status code.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \retval EALREADY - Operation already in progress.
 * \retval EISDIR - Is a directory.
 * \retval EBADF - Bad file number.
 * \retval EFBIG - File too large.
 * \warning It exits the application if called when no memory space is available.
 */
SG_EXTERN int sg_httpres_sendfile(struct sg_httpres *res, size_t block_size, uint64_t max_size, const char *filename,
                                  bool rendered, unsigned int status);

/**
 * Sends a stream to the client.
 * \param[in] res Response handle.
 * \param[in] size Size of the stream.
 * \param[in] block_size Preferred block size for stream loading.
 * \param[in] read_cb Callback to read data from stream handle.
 * \param[in] handle Stream handle.
 * \param[in] free_cb Callback to free the stream handle.
 * \param[in] status HTTP status code.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \retval EALREADY - Operation already in progress.
 * \note Use `size = 0` if the stream size is unknown.
 * \warning It exits the application if called when no memory space is available.
 */
SG_EXTERN int sg_httpres_sendstream(struct sg_httpres *res, uint64_t size, size_t block_size, sg_read_cb read_cb,
                                    void *handle, sg_free_cb free_cb, unsigned int status);

/**
 * Clears all headers, cookies, status and internal buffers of the response handle.
 * \param[in] res Response handle.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 */
SG_EXTERN int sg_httpres_clear(struct sg_httpres *res);

/**
 * Creates a new HTTP server handle.
 * \param[in] auth_cb Callback to grant/deny user access to the server resources.
 * \param[in] auth_cls User-defined closure for \pr{auth_cb}.
 * \param[in] req_cb Callback to handle requests and responses.
 * \param[in] req_cls User-defined closure for \pr{req_cb}.
 * \param[in] err_cb Callback to handle server errors.
 * \param[in] err_cls User-defined closure for \pr{err_cb}.
 * \return New HTTP server handle.
 * \retval NULL If the \pr{req_cb} or \pr{err_cb} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN struct sg_httpsrv *sg_httpsrv_new2(sg_httpauth_cb auth_cb, void *auth_cls, sg_httpreq_cb req_cb,
                                             void *req_cls, sg_err_cb err_cb, void *err_cls)
__SG_MALLOC;

/**
 * Creates a new HTTP server handle.
 * \param[in] cb Callback to handle requests and responses.
 * \param[in] cls User-defined closure.
 * \return New HTTP server handle.
 * \retval NULL If the \pr{cb} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN struct sg_httpsrv *sg_httpsrv_new(sg_httpreq_cb cb, void *cls)
__SG_MALLOC;

/**
 * Frees the server handle previously allocated by #sg_httpsrv_new() or #sg_httpsrv_new2().
 * \param[in] srv Pointer of the server to be freed.
 * \note If the server is running it stops before being freed.
 */
SG_EXTERN void sg_httpsrv_free(struct sg_httpsrv *srv);

#ifdef SG_HTTPS_SUPPORT

/**
 * Starts the HTTPS server.
 * \param[in] srv Server handle.
 * \param[in] key Memory pointer for the private key (key.pem) to be used by the HTTPS server.
 * \param[in] pwd Password for the private key.
 * \param[in] cert Memory pointer for the certificate (cert.pem) to be used by the HTTPS server.
 * \param[in] trust Memory pointer for the certificate (ca.pem) to be used by the HTTPS server for
 * client authentication.
 * \param[in] dhparams Memory pointer for the Diffie Hellman parameters (dh.pem) to be used by the HTTPS server for
 * key exchange.
 * \param[in] port Port for listening to connections.
 * \param[in] threaded Enable/disable the threaded model. If `true`, the server creates one thread per connection.
 * \return `true` if the server is started, `false` otherwise. If \pr{srv} is null, sets the `errno` to `EINVAL`.
 * \note If port is `0`, the operating system will assign an unused port randomly.
 */
SG_EXTERN bool sg_httpsrv_tls_listen2(struct sg_httpsrv *srv, const char *key, const char *pwd, const char *cert,
                                      const char *trust, const char *dhparams, uint16_t port, bool threaded);

/**
 * Starts the HTTPS server.
 * \param[in] srv Server handle.
 * \param[in] key Memory pointer for the private key (key.pem) to be used by the HTTPS server.
 * \param[in] cert Memory pointer for the certificate (cert.pem) to be used by the HTTPS server.
 * \param[in] port Port for listening to connections.
 * \param[in] threaded Enable/disable the threaded model. If `true`, the server creates one thread per connection.
 * \return `true` if the server is started, `false` otherwise. If \pr{srv} is null, sets the `errno` to `EINVAL`.
 * \note If port is `0`, the operating system will assign an unused port randomly.
 */
SG_EXTERN bool sg_httpsrv_tls_listen(struct sg_httpsrv *srv, const char *key, const char *cert,
                                     uint16_t port, bool threaded);

#endif

/**
 * Starts the HTTP server.
 * \param[in] srv Server handle.
 * \param[in] port Port for listening to connections.
 * \param[in] threaded Enable/disable the threaded model. If `true`, the server creates one thread per connection.
 * \return `true` if the server is started, `false` otherwise. If \pr{srv} is null, sets the `errno` to `EINVAL`.
 * \note If port is `0`, the operating system will assign randomly an unused port.
 */
SG_EXTERN bool sg_httpsrv_listen(struct sg_httpsrv *srv, uint16_t port, bool threaded);

/**
 * Stops the server not to accept new connections.
 * \param[in] srv Server handle.
 * \return `0` if the server is stopped. If \pr{srv} is null, sets the `errno` to `EINVAL`.
 */
SG_EXTERN int sg_httpsrv_shutdown(struct sg_httpsrv *srv);

/**
 * Returns the server listening port.
 * \param[in] srv Server handle.
 * \return Server listening port, `0` otherwise. If \pr{srv} is null, sets the `errno` to `EINVAL`.
 */
SG_EXTERN uint16_t sg_httpsrv_port(struct sg_httpsrv *srv);

/**
 * Checks if the server was started in threaded model.
 * \param[in] srv Server handle.
 * \return `true` if the server is in threaded model, `false` otherwise. If \pr{srv} is null, sets the `errno` to
 * `EINVAL`.
 */
SG_EXTERN bool sg_httpsrv_is_threaded(struct sg_httpsrv *srv);

/**
 * Sets the server uploading callbacks.
 * \param[in] srv Server handle.
 * \param[in] cb Callback to handle uploaded files and/or fields.
 * \param[in] cls User-defined closure.
 * \param[in] write_cb Callback to write the stream of the uploaded files.
 * \param[in] free_cb Callback to free stream of the uploaded files.
 * \param[in] save_cb Callback to save the uploaded files.
 * \param[in] save_as_cb Callback to save the uploaded files defining their path.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 */
SG_EXTERN int sg_httpsrv_set_upld_cbs(struct sg_httpsrv *srv, sg_httpupld_cb cb, void *cls, sg_write_cb write_cb,
                                      sg_free_cb free_cb, sg_save_cb save_cb, sg_save_as_cb save_as_cb);

/**
 * Sets the directory to save the uploaded files.
 * \param[in] srv Server handle.
 * \param[in] dir Directory as null-terminated string.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 */
SG_EXTERN int sg_httpsrv_set_upld_dir(struct sg_httpsrv *srv, const char *dir);

/**
 * Gets the directory of the uploaded files.
 * \param[in] srv Server handle.
 * \return Directory as null-terminated string.
 * \retval NULL If the \pr{srv} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN const char *sg_httpsrv_upld_dir(struct sg_httpsrv *srv);

/**
 * Sets a size to the post buffering.
 * \param[in] srv Server handle.
 * \param[in] size Post buffering size.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 */
SG_EXTERN int sg_httpsrv_set_post_buf_size(struct sg_httpsrv *srv, size_t size);

/**
 * Gets the size of the post buffering.
 * \param[in] srv Server handle.
 * \return Post buffering size.
 * \retval 0 If the \pr{srv} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN size_t sg_httpsrv_post_buf_size(struct sg_httpsrv *srv);

/**
 * Sets a limit to the total payload.
 * \param[in] srv Server handle.
 * \param[in] limit Payload total limit.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 */
SG_EXTERN int sg_httpsrv_set_payld_limit(struct sg_httpsrv *srv, size_t limit);

/**
 * Gets the limit of the total payload.
 * \param[in] srv Server handle.
 * \return Payload total limit.
 * \retval 0 If the \pr{srv} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN size_t sg_httpsrv_payld_limit(struct sg_httpsrv *srv);

/**
 * Sets a limit to the total uploads.
 * \param[in] srv Server handle.
 * \param[in] limit Uploads total limit.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 */
SG_EXTERN int sg_httpsrv_set_uplds_limit(struct sg_httpsrv *srv, uint64_t limit);

/**
 * Gets the limit of the total uploads.
 * \param[in] srv Server handle.
 * \return Uploads total limit.
 * \retval 0 If the \pr{srv} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN uint64_t sg_httpsrv_uplds_limit(struct sg_httpsrv *srv);

/**
 * Sets the size for the thread pool.
 * \param[in] srv Server handle.
 * \param[in] size Thread pool size.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 */
SG_EXTERN int sg_httpsrv_set_thr_pool_size(struct sg_httpsrv *srv, unsigned int size);

/**
 * Gets the size of the thread pool.
 * \param[in] srv Server handle.
 * \return Thread pool size.
 * \retval 0 If the \pr{srv} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN unsigned int sg_httpsrv_thr_pool_size(struct sg_httpsrv *srv);

/**
 * Sets the inactivity time to a client get time out.
 * \param[in] srv Server handle.
 * \param[in] timeout Timeout (in seconds).
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 */
SG_EXTERN int sg_httpsrv_set_con_timeout(struct sg_httpsrv *srv, unsigned int timeout);

/**
 * Gets the inactivity time to a client get time out.
 * \param[in] srv Server handle.
 * \return Timeout (in seconds).
 * \retval 0 If the \pr{srv} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN unsigned int sg_httpsrv_con_timeout(struct sg_httpsrv *srv);

/**
 * Sets the limit of concurrent connections.
 * \param[in] srv Server handle.
 * \param[in] limit Concurrent connections limit.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 */
SG_EXTERN int sg_httpsrv_set_con_limit(struct sg_httpsrv *srv, unsigned int limit);

/**
 * Gets the limit of concurrent connections.
 * \param[in] srv Server handle.
 * \return Concurrent connections limit.
 * \retval 0 If the \pr{srv} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN unsigned int sg_httpsrv_con_limit(struct sg_httpsrv *srv);

/**
 * Returns a value to end a stream reading processed by #sg_httpres_sendstream().
 * \param[in] err `true` to return a value indicating a stream reading error.
 * \return Value to end a stream reading.
 */
SG_EXTERN ssize_t sg_httpread_end(bool err);

/** \} */

#ifdef SG_PATH_ROUTING

/**
 * \ingroup sg_api
 * \defgroup sg_routing Path routing
 * High-performance path routing.
 * \{
 */

/**
 * Handle for the entry-point handling. It defines an entry-point to a path or resource. For example, given a path
 * `/api1/customer`, the part considered as entry-point is `/api1`.
 * \struct sg_entrypoint
 */
struct sg_entrypoint;

/**
 * Returns the name of the entry-point handle \pr{entrypoint}.
 * \param[in] entrypoint Entry-point handle.
 * \return Entry-point name as null-terminated string.
 * \retval NULL If the \pr{entrypoint} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN const char *sg_entrypoint_name(struct sg_entrypoint *entrypoint);

/**
 * Sets user data to the entry-point handle.
 * \param[in] entrypoint Entry-point handle.
 * \param[in] data User data pointer.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 */
SG_EXTERN int sg_entrypoint_set_user_data(struct sg_entrypoint *entrypoint, void *data);

/**
 * Gets user data from the entry-point handle.
 * \param[in] entrypoint Entry-point handle.
 * \return User data pointer.
 * \retval NULL If \pr{entrypoint} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN void *sg_entrypoint_user_data(struct sg_entrypoint *entrypoint);

/**
 * Handle for the entry-point list. It is used to create a list of entry-point #sg_entrypoint.
 * \struct sg_entrypoints
 */
struct sg_entrypoints;

/**
 * Callback signature used by #sg_entrypoints_iter() to iterate entry-point items.
 * \param[out] cls User-defined closure.
 * \param[out] pair Current iterated entry-point.
 * \retval 0 - Success.
 * \retval E<ERROR> - User-defined error to stop the items iteration.
 */
typedef int (*sg_entrypoints_iter_cb)(void *cls, struct sg_entrypoint *entrypoint);

/**
 * Creates a new entry-points handle.
 * \return Entry-points handle.
 * \warning It exits the application if called when no memory space is available.
 */
SG_EXTERN struct sg_entrypoints *sg_entrypoints_new(void)
__SG_MALLOC;

/**
 * Frees the entry-points handle previously allocated by #sg_entrypoints_new().
 * \param[in] entrypoints Pointer of the entry-points to be freed.
 */
SG_EXTERN void sg_entrypoints_free(struct sg_entrypoints *entrypoints);

/**
 * Adds a new entry-point item to the entry-points \pr{entrypoints}.
 * \param[in] entrypoints Entry-points handle.
 * \param[in] path Entry-point path.
 * \param[in] user_data User data pointer.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \retval EALREADY - Entry-point already added.
 * \warning It exits the application if called when no memory space is available.
 */
SG_EXTERN int sg_entrypoints_add(struct sg_entrypoints *entrypoints, const char *path, void *user_data);

/**
 * Removes an entry-point item from the entry-points \pr{entrypoints}.
 * \param[in] entrypoints Entry-points handle.
 * \param[in] path Entry-point path to be removed.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \retval ENOENT - Entry-point already removed.
 * \warning It exits the application if called when no memory space is available.
 */
SG_EXTERN int sg_entrypoints_rm(struct sg_entrypoints *entrypoints, const char *path);

/**
 * Iterates over entry-point items.
 * \param[in] entrypoints Entry-points handle.
 * \param[in] cb Callback to iterate the entry-point items.
 * \param[in,out] cls User-specified value.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \return Callback result when it is different from `0`.
 */
SG_EXTERN int sg_entrypoints_iter(struct sg_entrypoints *entrypoints, sg_entrypoints_iter_cb cb, void *cls);

/**
 * Cleans all existing entry-point items in the entry-points \pr{entrypoints}.
 * \param[in] entrypoints Entry-points handle.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 */
SG_EXTERN int sg_entrypoints_clear(struct sg_entrypoints *entrypoints);

/**
 * Finds an entry-point item by path.
 * \param[in] entrypoints Entry-points handle.
 * \param[in,out] entrypoint Pointer of the variable to store the found entry-point.
 * \param[in] path Entry-point path to be found.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \retval ENOENT - Pair not found.
 * \warning It exits the application if called when no memory space is available.
 */
SG_EXTERN int sg_entrypoints_find(struct sg_entrypoints *entrypoints, struct sg_entrypoint **entrypoint,
                                  const char *path);

/**
 * Handle for the route item. It holds a user data to be dispatched when a path matches the user defined
 * pattern (route pattern).
 * \struct sg_route
 */
struct sg_route;

/**
 * Callback signature used by #sg_route_get_segments() to iterate the path segments.
 * \param[out] cls User-defined closure.
 * \param[out] segment Current iterated segment.
 * \retval 0 - Success.
 * \retval E<ERROR> - User-defined error to stop the segments iteration.
 */
typedef int (*sg_get_segments_cb)(void *cls, const char *segment);

/**
 * Callback signature used by #sg_route_get_vars() to iterate the path variables.
 * \param[out] cls User-defined closure.
 * \param[out] name Current iterated variable name.
 * \param[out] val Current iterated variable value.
 * \retval 0 - Success.
 * \retval E<ERROR> - User-defined error to stop the variables iteration.
 */
typedef int (*sg_get_vars_cb)(void *cls, const char *name, const char *val);

/**
 * Callback signature used to handle the path routing.
 * \param[out] cls User-defined closure.
 * \param[out] route Route handle.
 */
typedef void (*sg_route_cb)(void *cls, struct sg_route *route);

/**
 * Callback signature used by #sg_routes_iter() to iterate route items.
 * \param[out] cls User-defined closure.
 * \param[out] route Current iterated route.
 * \retval 0 - Success.
 * \retval E<ERROR> - User-defined error to stop the route items iteration.
 */
typedef int (*sg_routes_iter_cb)(void *cls, struct sg_route *route);

/**
 * Returns the PCRE2 handle containing the compiled regex code.
 * \param[in] route Route handle.
 * \return PCRE2 handle containing the compiled regex code.
 * \retval NULL If \pr{route} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN void *sg_route_handle(struct sg_route *route);

/**
 * Returns the PCRE2 match data created from the route pattern.
 * \param[in] route Route handle.
 * \return PCRE2 match data.
 * \retval NULL If \pr{route} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN void *sg_route_match(struct sg_route *route);

/**
 * Returns the raw route pattern. For example, given a pattern `/foo`, the raw pattern is `^/foo$`.
 * \param[in] route Route handle.
 * \return Raw pattern as null-terminated string.
 * \retval NULL If \pr{route} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN const char *sg_route_rawpattern(struct sg_route *route);

/**
 * Returns the route pattern.
 * \param[in] route Route handle.
 * \return Pattern as null-terminated string.
 * \retval NULL If \pr{route} is null and sets the `errno` to `EINVAL`.
 * \retval NULL If no memory space available and sets the `errno` to `ENOMEM`.
 * \warning The caller must free the returned value.
 */
SG_EXTERN char *sg_route_pattern(struct sg_route *route)
__SG_MALLOC;

/**
 * Returns the route path.
 * \param[in] route Route handle.
 * \return Path component as null-terminated string.
 * \retval NULL If \pr{route} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN const char *sg_route_path(struct sg_route *route);

/**
 * Iterates over path segments.
 * \param[in] route Route handle.
 * \param[in] cb Callback to iterate the path segments.
 * \param[in,out] cls User-specified value.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \return Callback result when it is different from `0`.
 */
SG_EXTERN int sg_route_get_segments(struct sg_route *route, sg_get_segments_cb cb, void *cls);

/**
 * Iterates over path variables.
 * \param[in] route Route handle.
 * \param[in] cb Callback to iterate the path variables.
 * \param[in,out] cls User-specified value.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \return Callback result when it is different from `0`.
 */
SG_EXTERN int sg_route_get_vars(struct sg_route *route, sg_get_vars_cb cb, void *cls);

/**
 * Gets user data from the route handle.
 * \param[in] route Route handle.
 * \return User data pointer.
 * \retval NULL If \pr{route} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN void *sg_route_user_data(struct sg_route *route);

/**
 * Adds a route item to the route list \pr{routes}.
 * \param[in,out] routes Route list pointer to add a new route item.
 * \param[in,out] route Pointer of the variable to store the added route reference.
 * \param[in] pattern Pattern as null-terminated string. It must be a valid regular expression in PCRE2 syntax.
 * \param[in,out] errmsg Pointer of a string to store the error message.
 * \param[in] errlen Length of the error message.
 * \param[in] cb Callback to handle the path routing.
 * \param[in] cls User-defined closure.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \retval EALREADY - Route already added.
 * \note The pattern is enclosed between `^` and `$` automatically if it does not start with `(`.
 * \note The escape sequence \\K is not supported. It causes `EINVAL` if used.
 * \note The pattern is compiled using just-in-time optimization (JIT) when it is supported.
 * \warning It exits the application if called when no memory space is available.
 */
SG_EXTERN int sg_routes_add2(struct sg_route **routes, struct sg_route **route, const char *pattern,
                             char *errmsg, size_t errlen, sg_route_cb cb, void *cls);

/**
 * Adds a route item to the route list \pr{routes}. It uses the `stderr` to print the validation errors.
 * \param[in,out] routes Route list pointer to add a new route item.
 * \param[in] pattern Pattern as null-terminated string. It must be a valid regular expression in PCRE2 syntax.
 * \param[in] cb Callback to handle the path routing.
 * \param[in] cls User-defined closure.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \retval EALREADY - Route already added.
 * \note The pattern is enclosed between `^` and `$` automatically if it does not start with `(`.
 * \note The escape sequence \\K is not supported. It causes `EINVAL` if used.
 * \note The pattern is compiled using just-in-time optimization (JIT) when it is supported.
 * \warning It exits the application if called when no memory space is available.
 */
SG_EXTERN bool sg_routes_add(struct sg_route **routes, const char *pattern, sg_route_cb cb, void *cls);

/**
 * Removes a route item from the route list \pr{routes}.
 * \param[in,out] routes Route list pointer to add a new route item.
 * \param[in] pattern Pattern as null-terminated string of the route to be removed.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \retval ENOENT - Route already removed.
 */
SG_EXTERN int sg_routes_rm(struct sg_route **routes, const char *pattern);

/**
 * Iterates over all the routes in the route list \pr{routes}.
 * \param[in] routes Route list handle.
 * \param[in] cb Callback to iterate over route items.
 * \param[in] cls User-defined closure.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \retval E<ERROR> - User-defined error to abort the list iteration.
 */
SG_EXTERN int sg_routes_iter(struct sg_route *routes, sg_routes_iter_cb cb, void *cls);

/**
 * Returns the next route int the route list.
 * \param[in,out] route Pointer to the next route item.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 */
SG_EXTERN int sg_routes_next(struct sg_route **route);

/**
 * Counts the total routes in the route list \pr{routes}.
 * \param[in] routes Route list handle.
 * \return Total of route items.
 * \retval 0 If the list is empty or null.
 */
SG_EXTERN unsigned int sg_routes_count(struct sg_route *routes);

/**
 * Cleans the entire route list.
 * \param[in,out] routes Pointer to the route list.
 */
SG_EXTERN int sg_routes_cleanup(struct sg_route **routes);

/**
 * Handle for the path router. It holds the reference of a route list to be dispatched.
 * \struct sg_router
 */
struct sg_router;

/**
 * Callback signature used by #sg_router_dispatch2 in the route dispatching loop.
 * \param[out] cls User-defined closure.
 * \param[out] path Route path as null-terminated string.
 * \param[out] route Route handle.
 * \retval 0 - Success.
 * \retval E<ERROR> - User-defined error to stop the route dispatching loop.
 */
typedef int (*sg_router_dispatch_cb)(void *cls, const char *path, struct sg_route *route);

/**
 * Callback signature used by #sg_router_dispatch2 when the path matches the pattern before the route dispatching.
 * \param[out] cls User-defined closure.
 * \param[out] route Route handle.
 * \retval 0 - Success.
 * \retval E<ERROR> - User-defined error to stop the route dispatching.
 */
typedef int (*sg_router_match_cb)(void *cls, struct sg_route *route);

/**
 * Creates a new path router handle. It requires a filled route list \pr{routes}.
 * \param[in] routes Route list handle.
 * \return New router handle.
 * \retval NULL If the \pr{routes} is null and sets the `errno` to `EINVAL`.
 */
SG_EXTERN struct sg_router *sg_router_new(struct sg_route *routes)
__SG_MALLOC;

/**
 * Frees the router handle previously allocated by #sg_router_new().
 * \param[in] router Router handle.
 */
SG_EXTERN void sg_router_free(struct sg_router *router);

/**
 * Dispatches a route that its pattern matches the path passed in \pr{path}.
 * \param[in] router Router handle.
 * \param[in] path Path to dispatch a route.
 * \param[in] user_data User data pointer to be hold by the route.
 * \param[in] dispatch_cb Callback triggered for each route item in the route dispatching loop.
 * \param[in] cls User-defined closure passed to the \pr{dispatch_cb} and \pr{match_cb} callbacks.
 * \param[in] match_cb Callback triggered when the path matches the route pattern.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \retval ENOENT - Route not found or path not matched.
 * \retval E<ERROR> - User-defined error in \pr{dispatch_cb} or \pr{match_cb}.
 * \note The route callback #sg_route_cb is triggered when the path matches the route pattern.
 * \note The match logic uses just-in-time optimization (JIT) when it is supported.
 */
SG_EXTERN int sg_router_dispatch2(struct sg_router *router, const char *path, void *user_data,
                                  sg_router_dispatch_cb dispatch_cb, void *cls, sg_router_match_cb match_cb);

/**
 * Dispatches a route that its pattern matches the path passed in \pr{path}.
 * \param[in] router Router handle.
 * \param[in] path Path to dispatch a route.
 * \param[in] user_data User data pointer to be hold by the route.
 * \retval 0 - Success.
 * \retval EINVAL - Invalid argument.
 * \retval ENOENT - Route not found or path not matched.
 * \retval E<ERROR> - User-defined error in \pr{dispatch_cb} or \pr{match_cb}.
 * \note The route callback #sg_route_cb is triggered when the path matches the route pattern.
 * \note The match logic uses just-in-time optimization (JIT) when it is supported.
 */
SG_EXTERN int sg_router_dispatch(struct sg_router *router, const char *path, void *user_data);

/** \} */

#endif

#ifdef __cplusplus
}
#endif

#endif /* SAGUI_H */
