# riley/venv/bin/python3

from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import Column, Integer, Float, Date, Text


Base = declarative_base()


class Data(Base):
    __tablename__ = 'Data'
    __table_args__ = {'sqlite_autoincrement': True}
    id = Column(Integer, primary_key=True, nullable=False)
    file_location = Column(Text)
    use = Column(Text)

    def __repr__(self):
        return f'File {self.file_location}'
