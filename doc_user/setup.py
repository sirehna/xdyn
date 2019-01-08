from setuptools import setup, find_packages

setup(
    name='report',
    version='0.0.0',
    description='report generator for xdyn',
    long_description='',
    url='http://www.sirehna.com',
    author='GJ',
    author_email='guillaume.jacquenot@sirehna.com',
    license='EPL2.0',
    classifiers=[
        #   3 - Alpha
        #   4 - Beta
        #   5 - Production/Stable
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'Topic :: Software Development :: Build Tools',
        'License :: EPL2.0',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.3',
        'Programming Language :: Python :: 3.4',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
    ],
    keywords='',
    packages=find_packages(exclude=['contrib', 'docs', 'tests*']),
    install_requires=['pandas',
                      'xlrd',
                      'xlwt',
                      'h5py',
                      'jinja2',
                      'matplotlib',
                      'PyYAML'],
    extras_require={
        'dev': ['check-manifest'],
        'test': ['coverage'],
    },
    package_data={},
    data_files=[],
    entry_points={
        'console_scripts': [],
    },
)
