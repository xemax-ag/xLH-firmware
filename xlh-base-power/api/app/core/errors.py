# Errors
# https://docs.python.org/3/tutorial/errors.html

class AppError(Exception):
    pass


class AppTypeError(AppError):
    pass


class AppTypeNoneError(AppError):
    pass


class AppUserLevelError(AppError):
    pass


class AppDbError(AppError):
    pass


class AppDbApiError(AppError):
    pass


class AppDbDuplicatedError(AppError):
    pass


class AppDbUpdateAlreadyDoneError(AppError):
    pass


class AppXlsSharepointError(AppError):
    pass


class AppXlsTypNotAvailable(AppError):
    pass

