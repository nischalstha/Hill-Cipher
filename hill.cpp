/**
 * A program that implements hill cipher algorithm
 * @author by Nischal Shrestha
 * @version date 4 September, 2017
 */

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>     /* exit, EXIT_FAILURE */

using namespace std;

// Finds the determinant of the matrix.
int findDet(vector<vector<int>> keyMatrix, int order )
{	
	int determinant1, determinant2, determinant3;
	int determinant;
	if (order == 2)
	{
		determinant = ((keyMatrix[0][0] * keyMatrix[1][1]) - (keyMatrix[1][0] * keyMatrix[0][1]));
		return determinant;
	}
	else if (order == 3)
	{
		determinant1 = (keyMatrix[1][1] * keyMatrix[2][2]) - (keyMatrix[2][1] * keyMatrix[1][2]); 
		determinant2 = (keyMatrix[1][0] * keyMatrix[2][2]) - (keyMatrix[2][0] * keyMatrix[1][2]); 
		determinant3 = (keyMatrix[1][0] * keyMatrix[2][1]) - (keyMatrix[2][0] * keyMatrix[1][1]);

		determinant = ((keyMatrix[0][0] * determinant1) - (keyMatrix[0][1] * determinant2) + (keyMatrix[0][2] * determinant3));
		return determinant;
	}
	return 0;
}

// Finds the inverse of a matrix. 
vector <vector <int>> findInverse ( vector<vector<int>> keyMatrix, int order, int determinant)
{
	vector<vector<int>> inverseMatrix(order,vector<int>(order));

	int each;
	int invDet;

	// If the determinant is negative, keep on adding 26 until it is positive.
	while (determinant < 0)
	{
		determinant += 26;
	}

	// Finds the inverse of the determinant.
	for (int i = 0 ; i < 26 ; i++)
	{
		// ax mod n = 1
		if ((determinant * i) % 26 == 1)
		{
			invDet =  i ;
		}
	}

 	cout << "inverse determinant: " << invDet << endl;

	if (order == 2)
	{
		// Swaping element of the matrix.
		swap(keyMatrix[0][0], keyMatrix[1][1]);

		// Changes sign of the element of the matrix.
		keyMatrix[0][1] = - (keyMatrix[0][1]);
		keyMatrix[1][0] = - (keyMatrix[1][0]);

		for (int i = 0 ; i < 2; i ++)
		{
			for (int j = 0; j < 2 ; j++)
			{
				each = keyMatrix[i][j];

				// if the element in matrix is negative, add 26 until positive.
				while (each < 0)
				{
					each += 26;
				}
				// multiply each element of matrix by determinant inverse and mod with 26.
				(inverseMatrix[i])[j] = (each * invDet) % 26;
			}
		}

	}

	else if (order == 3)
	{
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				each = ((keyMatrix[(j+1)%3][(i+1)%3] * 
						keyMatrix[(j+2)%3][(i+2)%3]) - 
						(keyMatrix[(j+1)%3][(i+2)%3] * 
						keyMatrix[(j+2)%3][(i+1)%3]));

				// if the element in matrix is negative, add 26 until positive.
				while (each < 0)
				{
					each += 26;
				}

				// multiply each element of matrix by determinant inverse and mod with 26.
				(inverseMatrix[i])[j] = (each * invDet) % 26;
			}
		}
	}
	return inverseMatrix;
}

int main()
{
	int choice;
	cout << "Would you like to encrypt or decrypt a message?" << endl ;
	cout << "1 - Encrypt message" << endl;
	cout << "2 - Decrypt Message" << endl;
	cin >> choice;

	if ( choice == 1 )
	{
		string input;
		int order;
		cout << "Enter the text to be encrypted (in lowercase without spaces): " ;
		cin.ignore();
		getline(cin, input);

		cout << "Enter the size of key matrix: " ;
		cin >> order;

		vector <vector<int>> keyMatrix(order,vector<int>(order));

		// Input a key matrix of either 2x2 or 3x3 order.
		cout << "Enter a inversible key matrix of " << order << "x" 
		<< order << " matrix: " ;

		for ( int i = 0 ; i < order ; i++)
		{
			for ( int j = 0 ; j < order ; j++)
			{
				cin >> keyMatrix[i][j];
			}
		}
		
		// Padding a letter x at the end if the input length is odd to make it even.
		for ( int i = 1 ; i <= input.length() % order ; i++ )
		{
			input += 'x';
		}

		string output = "";
		string subString = "";

		for ( int k = 1 ; k <= input.length() / order ; k++)
		{
			// Divides the string in two letters if order = 2, three letters if order = 3
			subString = input.substr( (k - 1) * order, k * order );

			vector<int> resultMatrix(order,0);

			for ( int i = 0 ; i < order ; i++ )
			{
				for ( int j = 0 ; j < order ; j++ )
				{	
					// Ciphertext = k * p mod 26.
					resultMatrix[i] = resultMatrix[i] + ( subString[j] - 'a') * keyMatrix[i][j];
					resultMatrix[i] %= 26;
				}
				output += ( resultMatrix[i] + 'a' );
			}
		}
		cout << "Encrypted text is: "<< output << endl;
	}	

	else if (choice == 2)
	{
		string input;
		int order;
		cout << "Enter the text to be decrypted (in lowercase without spaces): " ;
		cin.ignore();
		getline(cin, input);

		cout << "Enter the size of key matrix: " ;
		cin >> order;

		vector <vector<int>> keyMatrix(order,vector<int>(order));

		vector <vector<int>> inverseMatrix(order,vector<int>(order));

		// Input a key matrix of either 2x2 or 3x3 order.
		cout << "Enter a inversible key matrix of " << order << "x" << order << " order: " ;
		for ( int i = 0 ; i < order ; i++)
		{
			for ( int j = 0 ; j < order ; j++)
			{
				cin >> keyMatrix[i][j];
			}
		}

		// Padding a letter x at the end if the input length is odd to make it even.
		for ( int i = 1 ; i <= input.length() % order ; i++ )
		{
			input += 'x';
		}

		// Calls the determinant function to find the determinant
		int determinant = findDet(keyMatrix, order);

		cout << "determinant: " << determinant << endl;
		
		if (determinant == 0)
		{
			cout << "Invalid matrix. No inverse!" << endl;
			exit (EXIT_FAILURE);
		}

		inverseMatrix = findInverse(keyMatrix, order, determinant);

		string output = "";
		string subString = "";

		for ( int k = 1 ; k <= input.length() / order ; k++ )
		{
			// Divides the string in two letters if order = 2, three letters if order = 3
			subString = input.substr (( k - 1 ) * order , k * order );

			// cout << "each string: " << subString << endl;
			vector<int> resultMatrix(order , 0);

			for ( int i = 0 ; i < order ; i++ )
			{
				for ( int j = 0 ; j < order ; j++ )
				{
					// converts each letter in the divied string to it's numeric value and multipy with element in matrix.
					resultMatrix[i] +=  inverseMatrix[i][j] * ( subString[j] - 'a' ) ;
					// each value is mod by 26.
					resultMatrix[i] %= 26;
				}
				output += (resultMatrix[i] + 'a');
			}
			
		}
		cout << "Decrypted text is: "<< output << endl ;
	}
	else
	{
		cout << "Bad choice! Terminating program." << endl ;
	}
	return 0;
}