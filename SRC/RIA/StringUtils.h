#ifndef __STRINGUTILS_H_
#define __STRINGUTILS_H_

#if defined(UNICODE) || defined(_UNICODE)
typedef wstring TString;
#else
typedef string TString;
#endif

typedef list<TString> SplitResultList;
typedef vector<TString> SplitResultVector;

/*
将input字符串以delimiter分隔符，delimiter可以有多个字符，!!!返回找到的分隔符的个数!!!
*/
template< typename T >
int SplitString(const TString& input, const TString& delimiter, T& results, bool includeEmpties = true)
{
    TString::size_type iPos = 0;
    TString::size_type newPos = -1;
    TString::size_type sizeS2 = 1;//delimiter.size();
    TString::size_type isize = input.size();

    if( 
        ( isize == 0 )
        ||
        ( sizeS2 == 0 )
        )
    {
        return 0;
    }

    vector<TString::size_type> positions;

    newPos = input.find_first_of(delimiter, 0);

    if( newPos == TString::npos )
    { 
        results.clear();
        results.push_back(input);
        return 0;
    }

    int numFound = 0;

    while( (newPos != TString::npos) && (newPos >= iPos) )
    {
        numFound++;
        positions.push_back(newPos);
        iPos = newPos;
        newPos = input.find_first_of(delimiter, iPos+sizeS2);
    }

    if( numFound == 0 )
    {
        return 0;
    }

    results.clear();

    for( TString::size_type i=0; i <= positions.size(); ++i )
    {
        TString s(_T(""));
        if( i == 0 ) 
        { 
            s = input.substr( i, positions[i] ); 
        }
        else
        {
            TString::size_type offset = positions[i-1] + sizeS2;
            if( offset < isize )
            {
                if( i == positions.size() )
                {
                    s = input.substr(offset);
                }
                else if( i > 0 )
                {
                    s = input.substr( positions[i-1] + sizeS2, positions[i] - positions[i-1] - sizeS2 );
                }
            }
        }
        if( includeEmpties || ( s.size() > 0 ) )
        {
            results.push_back(s);
        }
    }
    return numFound;
}

void HtmlDecode(__inout CString& strText);

#endif
